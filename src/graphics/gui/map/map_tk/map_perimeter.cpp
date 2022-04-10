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
void MapPerimeter::line_between(const xapmap::CurState &prog, const parsers::CoordPair &,
    const parsers::CoordPair &) const noexcept {
    const auto [r, g, b, a] = dflt::AIRPORT_BOUNDS_COLOR.get_rgb();

    cairo_set_source_rgba(prog.cr, r, g, b, a);
}

void MapPerimeter::internal_draw(const xapmap::CurState &prog, const parsers::AirportData &aptd) {
    const auto bounds_sz = aptd.bounds.size();
    if (bounds_sz <= 1) {
        throw std::runtime_error("No valid bounds for the airport " + aptd.name);
    }

    for (std::size_t i = 0; i < (bounds_sz - 1); ++i) {
        line_between(prog, aptd.bounds[i], aptd.bounds[i + 1]);
    }
    // Note: first point is both starting and ending location
    line_between(prog, aptd.bounds[bounds_sz - 1], aptd.bounds[0]);
}
}  // namespace graphics