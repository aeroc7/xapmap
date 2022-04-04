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
void TaskBar::draw(const xapmap::CurState &prog, nk_context *ctx) {
    const auto true_single_row_h = static_cast<int>(dflt::SINGLE_ROW_HEIGHT * prog.window_res_mult);
    if (nk_begin(ctx, "Taskbar",

            nk_recti(
                0, prog.window_height - true_single_row_h, prog.window_width, true_single_row_h),
            NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_static(ctx, true_single_row_h, prog.window_width / 6, 5);
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