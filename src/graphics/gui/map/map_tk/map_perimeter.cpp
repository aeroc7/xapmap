/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "map_perimeter.h"

#include <config/defaults.h>

namespace graphics {
void line_between(
    const xapmap::CurState &, const parsers::CoordPair &, const parsers::CoordPair &) noexcept {
}

void MapPerimeter::internal_draw(const xapmap::CurState &, const parsers::AirportData &) {
}
}  // namespace graphics