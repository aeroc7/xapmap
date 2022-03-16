/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "nuklear_cairo_color.h"

#include "nuklear_include.h"

namespace nk_impl {
RgbColor::RgbColor(const nk_color &col) {
    r = static_cast<double>(col.r) / 255.0;
    g = static_cast<double>(col.g) / 255.0;
    b = static_cast<double>(col.b) / 255.0;
    a = static_cast<double>(col.a) / 255.0;
}
}  // namespace nk_impl