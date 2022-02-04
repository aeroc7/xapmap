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

#include <exception>
#include <fstream>
#include <functional>

using namespace utils;

namespace parsers {
class ParseError : public std::exception {
public:
    ParseError(const std::string &s, SourceLocation sl = SourceLocation::current()) {
        msg = "\nError: " + s + "\nLoc: " + sl.get_filename() + ":" + sl.get_function() + ":" +
              std::to_string(sl.get_line());
    }

    const char *what() const noexcept {
        return msg.c_str();
    }

private:
    std::string msg;
};

class FileLineReader {
public:
    using cb_signature = void(const std::string &);
    FileLineReader(const std::string &path, std::function<cb_signature> cb_on_line)
        : on_line(cb_on_line) {
        read_file(path);
    }

private:
    void read_file(const std::string &path) const {
        std::ifstream file{path};
        std::string line_buf;
        line_buf.reserve(LINE_BUF_SIZE_INIT);

        if (!file) {
            throw ParseError("Failed to open file: " + path);
        }

        while (std::getline(file, line_buf)) {
            on_line(line_buf);
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
};

ParseAptDat::ParseAptDat(const std::string &path) {
    const auto sd = scenery_directories(path);
    (void)(sd);
}

std::vector<std::string> ParseAptDat::scenery_directories(const std::string &path) const {
    std::vector<std::string> file_paths;
    FileLineReader(path + "scenery_packs.ini", [&](const std::string &line) {
        const auto row_item = split_string(line, 0);

        if (!row_item.success) {
            throw ParseError("Failed to split line: '" + line + "' into chunks");
        }

        if (row_item.str == "SCENERY_PACK") {
            Log() << line;
        }
    });
    return {};
}
}  // namespace parsers