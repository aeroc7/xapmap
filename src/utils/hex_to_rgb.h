/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef HEX_TO_RGB_H_
#define HEX_TO_RGB_H_

#include <tuple>

namespace utils {
constexpr auto hex_to_rgb(unsigned color) {
    return std::make_tuple<double, double, double>(
        ((color >> 16) & 0xFF) / 255.0, ((color >> 8) & 0xFF) / 255.0, (color & 0xFF) / 255.0);
}
}  // namespace utils

#endif /* HEX_TO_RGB_H_ */