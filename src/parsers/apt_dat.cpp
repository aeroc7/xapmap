/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "apt_dat.h"

#include <utils/log.h>
#include <utils/str_utils.h>
#include <utils/from_chars_full.h>

#include <charconv>
#include <chrono>
#include <exception>
#include <fstream>
#include <typeinfo>

using namespace utils;

namespace parsers {
class ParseError : public std::exception {
public:
    ParseError(const std::string &str, SourceLocation sl = SourceLocation::current()) {
        write_msg(str, sl);
    }

    ParseError(const std::string &str, const std::string &filename, std::size_t line_num,
        SourceLocation sl = SourceLocation::current()) {
        write_msg_w_fninfo(str, filename, line_num, sl);
    }

    const char *what() const noexcept {
        return msg.c_str();
    }

private:
    void write_msg(const std::string &s, const SourceLocation &sl) {
        msg = "\nErr Info: " + s + "\nSource Loc: " + sl.get_filename() + ":" + sl.get_function() +
              ":" + std::to_string(sl.get_line());
    }

    void write_msg_w_fninfo(const std::string &str, const std::string &filename,
        std::size_t file_linenum, const SourceLocation &sl) {
        write_msg(str, sl);
        msg += "\nFile: " + filename + ":" + std::to_string(file_linenum);
    }

    std::string msg;
};

static inline void only_raise_info(const ParseError &e) {
    Log(Log::ERROR) << "Parse error: " << e.what();
}

template <typename T>
static T num_from_str(std::string_view str, const LrCbParam &p) {
    T out_val;
    const auto [ptr, ec] = utils::from_chars<T>(str.begin(), str.end(), out_val);

    switch (ec) {
        case std::errc::invalid_argument:
            only_raise_info(
                ParseError("Failed to convert '" + std::string(str) + "' to numerical type.",
                    p.filename, p.line_num));
            break;
        case std::errc::result_out_of_range:
            only_raise_info(ParseError("The number " + std::string(str) +
                                           " is out of range for the type " + typeid(T).name(),
                p.filename, p.line_num));
            break;
        default:
            break;
    }

    return out_val;
}

static bool sv_check_good(const str::ss_return &s, const LrCbParam &p) {
    using cu_str_err = str::ss_error;
    switch (s.err) {
        case cu_str_err::out_of_range:
            only_raise_info(
                ParseError("Index-string-length mismatch (out of range)", p.filename, p.line_num));
            return false;
        case cu_str_err::str_empty:
            only_raise_info(ParseError("String was empty", p.filename, p.line_num));
            return false;
        case cu_str_err::success:
        default:
            return true;
    }
}

static inline std::string_view set_default_str_ne(
    const str::ss_return &str_val, const LrCbParam &p) {
    if (sv_check_good(str_val, p)) {
        return str_val.str;
    }

    return "";
}

template <typename Lambda>
class FileLineReader {
public:
    FileLineReader(const std::string &path, Lambda cb_on_line)
        : on_line(cb_on_line), file_basename(filename_split(path)) {
        read_file(path);
    }

private:
    void read_file(const std::string &path) const {
        std::ifstream file{path};
        std::string line_buf;
        line_buf.reserve(LINE_BUF_SIZE_INIT);
        std::size_t line_counter{};

        if (!file) {
            throw ParseError("Failed to open file: " + path);
        }

        while (std::getline(file, line_buf)) {
            line_counter += 1;
            on_line(LrCbParam{
                .line = line_buf, .line_num = line_counter, .filename = this->file_basename});
        }

        if (file.eof()) {
            return;
        } else if (file.bad()) {
            throw ParseError("I/O error while reading file " + path);
        } else if (file.fail()) {
            throw ParseError("I/O stream read failed " + path);
        }
    }

    Lambda on_line;
    static constexpr std::size_t LINE_BUF_SIZE_INIT = 1024;
    std::string file_basename;
};

ParseAptDat::ParseAptDat(const std::string &path) {
    const auto sd = scenery_directories(path);

    for (const auto &e : sd) {
        parse_apt_dat_file(e);
    }
}

std::vector<std::string> ParseAptDat::scenery_directories(const std::string &path) const {
    std::vector<std::string> file_paths;

    FileLineReader(path + "scenery_packs.ini", [&](const LrCbParam &p) {
        const auto row_item = str::split_string_sv(p.line, 0);

        if (row_item.str == "SCENERY_PACK") {
            const auto file_loc = str::split_string_tend_sv(p.line, 1);
            switch (file_loc.err) {
                case str::ss_error::out_of_range:
                    only_raise_info(ParseError(
                        "Attempted string split-to-end is out of range (line contents: '" + p.line +
                            "')",
                        p.filename, p.line_num));
                    return;
                case str::ss_error::str_empty:
                    only_raise_info(ParseError(
                        "Tried to extract pathname but it was empty", p.filename, p.line_num));
                    return;
                case str::ss_error::success:
                    break;
            }

            file_paths.push_back(std::string{file_loc.str});
        }
    });

    return file_paths;
}

void ParseAptDat::cur_apt_start(const LrCbParam &p) {
    const auto icao_str = str::split_string_sv(p.line, 4);
    const auto ap_name_str = str::split_string_tend_sv(p.line, 5);

    cur_apt.icao = set_default_str_ne(icao_str, p);
    cur_apt.name = set_default_str_ne(ap_name_str, p);
}

void ParseAptDat::cur_apt_end(const LrCbParam &) {
    if (str::cmp_equal_sv(cur_apt.icao.c_str(), "") == false) {
        airport_db[cur_apt.icao] = cur_apt;
    }

    cur_apt = {};
}

void ParseAptDat::cur_apt_1302(const LrCbParam &p) {
    const auto data_category = str::split_string_sv(p.line, 1);

    auto split_after_safe = [](const LrCbParam &pline, unsigned index) {
        const auto split_val = str::split_string_tend_sv(pline.line, index);
        return set_default_str_ne(split_val, pline);
    };

    if (str::cmp_equal_sv(data_category.str, "city")) {
        cur_apt.city = split_after_safe(p, 2);
    } else if (str::cmp_equal_sv(data_category.str, "state")) {
        cur_apt.state = split_after_safe(p, 2);
    }
}

void ParseAptDat::cur_apt_100(const LrCbParam &) {
}

void ParseAptDat::parse_apt_dat_file(const std::string &file) {
    FileLineReader(
        "/home/bennett/X-Plane 11/" + file + "Earth nav data/apt.dat", [this](const LrCbParam &p) {
            const auto row_code_str = str::split_string_sv(p.line, 0);

            if (str::cmp_equal_sv(row_code_str.str, "1")) {
                // Land airport
                cur_apt_end(p);
                cur_apt_start(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "100")) {
                // Runway
                cur_apt_100(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "1302")) {
                // Airport metadata
                cur_apt_1302(p);
            } else if (str::cmp_equal_sv(row_code_str.str, "99")) {
                // End of file, finish up last airport
                cur_apt_end(p);
            }
        });
}
}  // namespace parsers