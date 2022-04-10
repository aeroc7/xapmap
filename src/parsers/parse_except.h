/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSE_EXCEPT_H_
#define PARSE_EXCEPT_H_

#include <utils/utils.h>

#include <string>

namespace parsers {
class ParseError : public std::exception {
public:
    ParseError(
        const std::string &str, utils::SourceLocation sl = utils::SourceLocation::current()) {
        write_msg(str, sl);
    }

    ParseError(const std::string &str, const std::string &filename, std::size_t line_num,
        utils::SourceLocation sl = utils::SourceLocation::current()) {
        write_msg_w_fninfo(str, filename, line_num, sl);
    }

    const char *what() const noexcept {
        return msg.c_str();
    }

private:
    void write_msg(const std::string &s, const utils::SourceLocation &sl) {
        msg = "\nErr Info: " + s + "\nSource Loc: " + sl.get_filename() + ":" + sl.get_function() +
              ":" + std::to_string(sl.get_line());
    }

    void write_msg_w_fninfo(const std::string &str, const std::string &filename,
        std::size_t file_linenum, const utils::SourceLocation &sl) {
        write_msg(str, sl);
        msg += "\nFile: " + filename + ":" + std::to_string(file_linenum);
    }

    std::string msg;
};
}  // namespace parsers

#endif /* PARSE_EXCEPT_H_ */