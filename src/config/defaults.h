/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef DEFAULTS_H_
#define DEFAULTS_H_

#include <graphics/gui/nuklear_impl/nuklear_cairo_color.h>

namespace dflt {
constexpr auto WINDOW_WIDTH = 730;
constexpr auto WINDOW_HEIGHT = 420;
constexpr auto WINDOW_FPS = 120;

constexpr auto SINGLE_ROW_HEIGHT = 35;  // px

constexpr nk_impl::RgbColor AIRPORT_BOUNDS_COLOR = {255, 255, 255, 255};
}  // namespace dflt

#endif /* DEFAULTS_H_ */