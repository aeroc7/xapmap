/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef FROM_CHARS_FULL_H_
#define FROM_CHARS_FULL_H_

#pragma GCC diagnostic push
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#include <fast_float/fast_float.h>
#pragma GCC diagnostic pop

#include <charconv>

namespace utils {
template <typename T>
auto from_chars(const char *first, const char *last, T &value) noexcept {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return fast_float::from_chars<T>(first, last, value);
    } else {
        return std::from_chars<T>(first, last, value);
    }
}
}  // namespace utils

#endif /* FROM_CHARS_FULL_H_ */