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
class Logger;

struct LoggerData {
    LoggerData(Logger &lg, std::source_location loc = std::source_location::current())
        : src_loc(loc), log_ref(lg) {}
    void write_info(std::ostream &os) {
        os << LOGGER_HEADER << ' ' << fmtd_time() << " [" << filename_split(src_loc.file_name())
           << ':' << src_loc.line() << "]: ";
    }
    Logger &lg() { return log_ref; }

private:
    std::string fmtd_time() {
        std::tm tbuf, *now;
        time_t t;
        char strbuf[16];

        t = std::time(nullptr);
        now = localtime_r(&t, &tbuf);
        std::strftime(strbuf, sizeof(strbuf), "%H:%M:%S", now);

        return strbuf;
    }

    constexpr std::string_view filename_split(std::string_view path) const noexcept {
        const auto last_path_sep = path.find_last_of("/");
        return std::string_view(path.data() + last_path_sep + 1, path.length() - last_path_sep);
    }

    std::source_location src_loc;
    Logger &log_ref;
};

class Logger {
public:
    Logger(std::ostream &o) : out_log(o) {}

    template <typename T>
    friend LoggerData operator<<(LoggerData ld, const T &msg) noexcept {
        std::ostream &out = ld.lg().out_log;
        ld.write_info(out);
        out << msg << '\n';
        return ld;
    }

    std::ostream &stream() { return out_log; }

private:
    std::ostream &out_log;
};

extern Logger log;
extern Logger log_err;
}  // namespace utils

#endif /* LOG_H_ */