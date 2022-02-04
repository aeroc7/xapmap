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

inline auto split_string(const std::string &str, unsigned index) {
    using iter_add_type = std::string::difference_type;
    const auto len = str.length();
    unsigned index_ctr = 0;

    struct split_string_return {
        std::string str;
        bool success;
    };

    for (std::size_t i = 0; i < (len - 1); ++i) {
        const auto cur_is_space = std::isspace(str[i]);
        if (cur_is_space && !std::isspace(str[i + 1])) {
            index_ctr += 1;
        }

        if (index_ctr == index) {
            std::string::const_iterator new_iter =
                (cur_is_space ? (str.begin() + static_cast<iter_add_type>(i + 1))
                              : str.begin() + static_cast<iter_add_type>(i));
            const auto next_space_pos =
                std::find_if(new_iter, str.end(), [](std::string::value_type c) {
                    return std::isspace(c);
                });
            return split_string_return{{new_iter, next_space_pos}, true};
        }
    }

    return split_string_return{{}, false};
}
}  // namespace utils

#endif /* STR_UTILS_H_ */