/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "map_cmp.h"

namespace graphics {
void MapCmp::draw(const xapmap::CurState &prog, const parsers::AirportData &data) {
    if (is_visible()) {
        internal_draw(prog, data);
    }
}

void MapCmp::show() noexcept {
    should_draw = true;
}

void MapCmp::hide() noexcept {
    should_draw = false;
}

bool MapCmp::is_visible() const noexcept {
    return should_draw;
}
}  // namespace graphics