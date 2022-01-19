/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LOG_H_
#define LOG_H_

#include <ctime>
#include <iostream>
#include <source_location>

namespace utils {
class Logger {
public:
    struct LoggerData {
        LoggerData(Logger &, std::source_location loc = std::source_location::current()) noexcept {
            *this << LOGGER_HEADER << ' ' << Logger::fmtd_time() << " ["
                  << filename_split(loc.file_name()) << ':' << loc.line() << "]: ";
        }

    private:
        constexpr std::string_view filename_split(std::string_view path) const noexcept {
            const auto last_path_sep = path.find_last_of("/");
            return std::string_view(path.data() + last_path_sep + 1, path.length() - last_path_sep);
        }
    };

    template <typename T>
    friend LoggerData operator<<(LoggerData ld, const T &msg) noexcept {
        std::cout << msg;
        return ld;
    }

private:
    static std::string fmtd_time() {
        std::tm tbuf, *now;
        time_t  t;
        char    strbuf[16];

        t = std::time(nullptr);
        now = localtime_r(&t, &tbuf);
        std::strftime(strbuf, sizeof(strbuf), "%H:%M:%S", now);

        return strbuf;
    }
};

extern Logger log;
}  // namespace utils

#endif /* LOG_H_ */