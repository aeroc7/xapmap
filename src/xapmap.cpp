/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "xapmap.h"

namespace xapmap {
Xapmap::Xapmap() {
    whdlr.create_window("xapmap", 750, 500);
    glewInit();
    graphics::CairoMt mt{500, 500, 60};
    mt.set_callbacks(
        [](cairo_t *) {
        },
        [](cairo_t *) {
        },
        [](cairo_t *) {
        });
    whdlr.window_loop([]() {
    });

    mt.stop_thread();
}
}  // namespace xapmap