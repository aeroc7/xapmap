/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LOG_H_
#define LOG_H_

#include <cstdint>
#include <iostream>

#include "pf_wrapper.h"
#include "str_utils.h"

namespace utils {
struct SourceLocation {
    constexpr SourceLocation(const char *f, const char *func, uint_least32_t l)
        : filename(f), function(func), line(l) {
    }
    constexpr const char *get_filename() const noexcept {
        return filename;
    }
    constexpr uint_least32_t get_line() const noexcept {
        return line;
    }
    constexpr const char *get_function() const noexcept {
        return function;
    }

    static constexpr SourceLocation current(
#if defined(__clang__) || defined(__GNUC__)
        const char *fileName = __builtin_FILE(), const char *functionName = __builtin_FUNCTION(),
        uint_least32_t line = __builtin_LINE()) noexcept {
#else
#error "unsupported";
#endif
        return SourceLocation(fileName, functionName, line);
    }

private:
    const char *filename;
    const char *function;
    uint_least32_t line;
};

class Log {
public:
    enum class LogType : uint8_t { Info = 0, Error };
    static constexpr auto INFO = LogType::Info;
    static constexpr auto ERROR = LogType::Error;

    Log(LogType logtype = INFO, SourceLocation slc = SourceLocation::current())
        : out_log(log_stream(logtype)) {
        out_log << LOGGER_HEADER << ' ' << formatted_logtime() << " ["
                << filename_split(slc.get_filename()) << ':' << slc.get_line() << "]: ";
    }

    ~Log() {
        out_log << '\n';
    }
    template <typename T>
    std::ostream &operator<<(const T &msg) noexcept {
        out_log << msg;
        return out_log;
    }

private:
    std::ostream &log_stream(LogType lt) const noexcept {
        switch (lt) {
            case LogType::Info:
                return std::cout;
            case LogType::Error:
                return std::cerr;
        }
    }

    std::ostream &out_log;
};

}  // namespace utils

#endif /* LOG_H_ */