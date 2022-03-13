/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "airport_search.h"

#include <graphics/gui/nuklear_impl/nuklear_include.h>

namespace graphics {
void AirportSearch::draw(const xapmap::CurState &prog, nk_context *ctx) {
    if (nk_begin(ctx, WINDOW_NAME,
            nk_recti(dflt::DEFAULT_WINDOW_WIDTH / 4, dflt::DEFAULT_WINDOW_HEIGHT / 4,
                dflt::DEFAULT_WINDOW_WIDTH / 2, dflt::DEFAULT_WINDOW_HEIGHT / 2),
            NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_DYNAMIC | NK_WINDOW_MOVABLE |
                NK_WINDOW_CLOSABLE)) {
        nk_layout_row_dynamic(ctx, dflt::DEFAULT_SINGLE_ROW_HEIGHT, 1);
        nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, input_buf.data(),
            static_cast<int>(input_buf.size()), nk_filter_ascii);
        nk_layout_row_dynamic(ctx, 2, 1);
        nk_layout_row_dynamic(ctx, dflt::DEFAULT_SINGLE_ROW_HEIGHT, 1);

        if (nk_button_label(ctx, "Load")) {
            const char *cur_apt = input_buf.data();
            if (prog.map_conf.ap_db_set_if_exists(cur_apt)) {
                airport_not_found = false;
                nk_window_show(ctx, WINDOW_NAME, NK_HIDDEN);
            } else {
                airport_not_found = true;
            }
        }

        if (airport_not_found) {
            nk_layout_row_dynamic(ctx, 4, 1);
            nk_layout_row_dynamic(ctx, dflt::DEFAULT_SINGLE_ROW_HEIGHT / 1.5f, 1);
            nk_text_colored(ctx, "Airport not found", 17, NK_TEXT_CENTERED,
                nk_color{.r = 255, .g = 0, .b = 0, .a = 255});
        }
    }

    nk_end(ctx);
}
}  // namespace graphics