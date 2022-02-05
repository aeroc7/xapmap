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
#include <functional>

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

class FileLineReader {
public:
    struct CbParam {
        const std::string &line;
        std::size_t line_num;
        const std::string &filename;
    };

    using cb_signature = void(const CbParam &p);
    FileLineReader(const std::string &path, std::function<cb_signature> cb_on_line)
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
            on_line({line_buf, line_counter, this->file_basename});
        }

        if (file.eof()) {
            return;
        } else if (file.bad()) {
            throw ParseError("I/O error while reading file " + path);
        } else if (file.fail()) {
            throw ParseError("I/O stream read failed " + path);
        }
    }

    std::function<cb_signature> on_line;
    static constexpr std::size_t LINE_BUF_SIZE_INIT = 1024;
    std::string file_basename;
};

ParseAptDat::ParseAptDat(const std::string &path) {
    [[maybe_unused]] const auto sd = scenery_directories(path);
}

std::vector<std::string> ParseAptDat::scenery_directories(const std::string &path) const {
    std::vector<std::string> file_paths{15};
    FileLineReader(path + "scenery_packs.ini", [&](const FileLineReader::CbParam &p) {
        const auto row_item = str::split_string(p.line, 0);

        if (row_item.str == "SCENERY_PACK") {
            const auto file_loc = str::split_string_tend(p.line, 1);
            switch (file_loc.err) {
                case str::ss_error::out_of_range:
                    throw ParseError(
                        "Attempted string split to end is out of range (line contents: '" + p.line +
                            "')",
                        p.filename, p.line_num);
                case str::ss_error::str_empty:
                    throw ParseError(
                        "Tried to extract pathname but it was empty", p.filename, p.line_num);
                case str::ss_error::success:
                    break;
            }

            file_paths.push_back(std::move(file_loc.str));
        }
    });
    return file_paths;
}
}  // namespace parsers