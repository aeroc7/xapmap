/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef STR_UTILS_H_
#define STR_UTILS_H_

#include <string_view>

namespace utils {
constexpr std::string_view filename_split(std::string_view path) noexcept {
    const std::size_t last_path_sep = [&path] {
        const auto ps = path.find_last_of("/");
        return (ps == std::string::npos ? 0 : (ps + 1));
    }();
    return std::string_view(path.data() + last_path_sep, path.length() - last_path_sep);
}

class str {
public:
    enum class ss_error { out_of_range, str_empty, success };
    struct ss_return {
        std::string_view str;
        ss_error err;
    };

    static constexpr auto SPACE_DELIM = ' ';

    template <typename CharType>
    static constexpr auto str_isspace(CharType chr) noexcept {
        switch (chr) {
            case ' ':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
            case '\v':
                return true;
            default:
                return false;
        }
    }

    // Returns string section from whitespace `index` to the end of the string
    static constexpr auto split_string_tend_sv(std::string_view str, std::size_t index) noexcept {
        const auto len = str.length();
        std::size_t index_ctr = 0;

        if (len == 0) {
            return ss_return{{}, ss_error::str_empty};
        }

        for (std::size_t i = 0; i < (len - 1); ++i) {
            const auto cur_is_space = str_isspace(str[i]);
            if (cur_is_space && !str_isspace(str[i + 1])) {
                index_ctr += 1;
            }

            if (index_ctr == index) {
                const std::string_view::const_iterator new_iter =
                    (cur_is_space ? (str.begin() + (i + 1)) : (str.begin() + i));
                return ss_return{{new_iter, str.end()}, ss_error::success};
            }
        }

        return ss_return{{}, ss_error::out_of_range};
    }

    // Returns string token at whitespace `index`.
    static constexpr auto split_string_sv(std::string_view str, unsigned index) noexcept {
        const auto len = str.length();
        std::size_t index_ctr{};

        if (len == 0) {
            return ss_return{{}, ss_error::str_empty};
        }

        for (std::size_t i = 0; i < (len - 1); ++i) {
            const auto cur_is_space = str_isspace(str[i]);
            if (cur_is_space && !str_isspace(str[i + 1])) {
                index_ctr += 1;
            }

            if (index_ctr == index) {
                const auto new_iter = (cur_is_space ? (str.begin() + (i + 1)) : (str.begin() + i));
                const auto next_space_pos = [&]() {
                    const char *it;
                    for (it = new_iter; it != str.end(); ++it) {
                        if (str_isspace(*it)) {
                            break;
                        }
                    }
                    return it;
                }();
                return ss_return{{new_iter, next_space_pos}, ss_error::success};
            }
        }

        return ss_return{{}, ss_error::out_of_range};
    }

    static constexpr bool cmp_equal_sv(std::string_view s1, std::string_view s2) noexcept {
        return (s1 == s2);
    }
};
}  // namespace utils

#endif /* STR_UTILS_H_ */