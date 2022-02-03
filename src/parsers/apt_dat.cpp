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

ParseAptDat::ParseAptDat(const std::string &path) {
    std::ifstream file{path};

    if (file.good()) {
    }

    if (file.bad()) {
        throw ParseError("I/O error while reading file " + path);
    } else if (file.fail()) {
        throw ParseError("I/O stream read failed " + path);
    }
}
}  // namespace parsers