/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <utils/from_chars_full.h>
#include <utils/utils.h>

#define NK_MEMSET std::memset
#define NK_MEMCPY std::memcpy
#define NK_SQRT   std::sqrt
#define NK_SIN    std::sin
#define NK_COS    std::cos
#define NK_STRTOD nk_impl::nk_utils::strtod
#define NK_DTOA   nk_impl::nk_utils::dtoa

#define NK_IMPLEMENTATION
#include "nuklear_include.h"

namespace nk_impl::nk_utils {
double strtod(const char *str, const char **endptr) noexcept {
    ASSERT(endptr != nullptr);
    ASSERT(*endptr != nullptr);
    std::string_view str_as_view{str};
    double out_val{};
    auto [ptr, ec] = utils::from_chars<double>(str_as_view.begin(), str_as_view.end(), out_val);
    *endptr = ptr;
    return out_val;
}

char *dtoa(char *str, double n) noexcept {
    std::array<char, 32> buf;
    auto [ptr, ec] = std::to_chars(buf.begin(), buf.end() - 1, n);

    if (ec == std::errc()) {
        std::string_view str_buf(buf.begin(), ptr);
        std::memcpy(str, str_buf.data(), str_buf.size());
        str[str_buf.size()] = '\0';
        return str;
    }

    str[0] = '\0';
    return str;
}
}  // namespace nk_impl::nk_utils
