/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSER_BASE_H_
#define PARSER_BASE_H_

#include <utils/utils.h>

#include <atomic>
#include <cerrno>
#include <cstring>
#include <functional>
#include <string>

namespace xapmap::parsers {
class ParserBase {
public:
    void parse(std::string path);
    virtual void on_line(const std::string &line) noexcept = 0;
    void cancel();
    virtual ~ParserBase() = default;

    static std::string split_by_space(const std::string &line, unsigned index) {
        const std::size_t len = line.length();
        const char *data = line.c_str();
        char str_arr[MAX_SPLIT_LEN];
        char *str, *token;
        unsigned ctr = 0;

        ASSERT(len < MAX_SPLIT_LEN);

        std::strncpy(str_arr, data, sizeof(str_arr) - 1);
        str_arr[std::strlen(data)] = '\0';

        str = str_arr;

        while ((token = strtok_r(str, " ", &str))) {
            if (ctr == index) {
                return std::string(token);
            }

            ++ctr;
        }

        throw std::runtime_error("Failed to find token at index");
    }

    static std::string split_after_space(const std::string &line, unsigned index) {
        unsigned ctr = 0;
        bool was_char = false;
        const char *data = line.c_str();

        for (size_t i = 0; i < strlen(data); ++i) {
            if (ctr == index) {
                if (isspace(data[i])) {
                    continue;
                }

                return std::string(data + i);
            }

            const bool is_space = isspace(data[i]);

            if (is_space && was_char) {
                ctr += 1;
                was_char = false;
            } else if (is_space) {
                continue;
            } else {
                was_char = true;
            }
        }

        throw std::runtime_error("Failed to find token after index");
    }

    template <typename T>
    static T i_from_chars(const std::string &str) {
        static_assert(std::is_same_v<long, T> || std::is_same_v<double, T>);
        T num{};
        const char *sdata = str.c_str();
        char *endptr;
        errno = 0;

        if constexpr (std::is_same_v<T, long>) {
            num = std::strtol(sdata, &endptr, 10);
        } else if constexpr (std::is_same_v<T, double>) {
            num = std::strtod(sdata, &endptr);
        }

        if (errno == ERANGE) {
            throw std::runtime_error("Str value is out of range");
        }

        return num;
    }

private:
    static constexpr auto MAX_SPLIT_LEN = 512;
    std::atomic_bool stop_parsing{false};
};
}  // namespace xapmap::parsers

#endif /* PARSER_BASE_H_ */