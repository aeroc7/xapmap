/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>

#include "pf_wrapper.h"

namespace utils {
class Log {
public:
    Log(std::ostream &os = std::cout) : out_log(os) {
        out_log << LOGGER_HEADER << ' ' << formatted_logtime() << " [" << filename_split(__FILE__)
                << ':' << __LINE__ << "]: ";
    }

    ~Log() { out_log << '\n'; }
    template <typename T>
    std::ostream &operator<<(const T &msg) noexcept {
        out_log << msg;
        return out_log;
    }

private:
    constexpr std::string_view filename_split(std::string_view path) const noexcept {
        const auto last_path_sep = path.find_last_of("/");
        return std::string_view(path.data() + last_path_sep + 1, path.length() - last_path_sep);
    }

    std::ostream &out_log;
};
}  // namespace utils

#endif /* LOG_H_ */