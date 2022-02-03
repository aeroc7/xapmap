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
}  // namespace utils

#endif /* STR_UTILS_H_ */