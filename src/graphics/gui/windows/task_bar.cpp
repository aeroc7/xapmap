/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "task_bar.h"

#include <graphics/gui/nuklear_impl/nuklear_include.h>

#include "airport_search.h"

namespace graphics {
void TaskBar::draw(const xapmap::CurState &, nk_context *ctx) {
    if (nk_begin(ctx, "Taskbar",
            nk_recti(0, dflt::WINDOW_HEIGHT - WINDOW_HEIGHT, dflt::WINDOW_WIDTH, WINDOW_HEIGHT),
            NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_static(ctx, 25, dflt::WINDOW_WIDTH / 8, 6);
        if (nk_button_label(ctx, "Lookup")) {
            if (nk_window_is_hidden(ctx, AirportSearch::WINDOW_NAME)) {
                nk_window_show(ctx, AirportSearch::WINDOW_NAME, NK_SHOWN);
            } else {
                nk_window_show(ctx, AirportSearch::WINDOW_NAME, NK_HIDDEN);
            }
        }
    }

    nk_end(ctx);
}
}  // namespace graphics