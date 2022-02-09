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

struct LrCbParam {
    const std::string &line;
    std::size_t line_num;
    const std::string &filename;
};

static inline void only_raise_info(const ParseError &e) {
    Log(Log::ERROR) << "Parse error: " << e.what();
}

template <typename T>
static T num_from_str(std::string_view str, const LrCbParam &p) {
    T out_val;
    const auto [ptr, ec] = std::from_chars<T>(str.begin(), str.end(), out_val);

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

void ParseAptDat::parse_apt_dat_file(const std::string &file) {
    FileLineReader(
        "/home/bennett/X-Plane 11/" + file + "Earth nav data/apt.dat", [this](const LrCbParam &p) {
            const auto row_code_str = str::split_string_sv(p.line, 0);

            if (str::cmp_equal_sv(row_code_str.str, "1")) {
                AirportData tmp;
                tmp.icao = str::split_string_sv(p.line, 4).str;
                tmp.city = str::split_string_tend_sv(p.line, 5).str;
                airport_db[tmp.icao] = tmp;
            }
        });
}
}  // namespace parsers