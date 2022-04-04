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
            nk_recti(prog.window_width / 4, prog.window_height / 4, prog.window_width / 2,
                prog.window_height / 2),
            NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_DYNAMIC | NK_WINDOW_MOVABLE |
                NK_WINDOW_CLOSABLE)) {
        nk_layout_row_dynamic(
            ctx, static_cast<float>(dflt::SINGLE_ROW_HEIGHT * prog.window_res_mult), 1);
        nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, input_buf.data(),
            static_cast<int>(input_buf.size()), nk_filter_ascii);
        nk_layout_row_dynamic(ctx, 2, 1);
        nk_layout_row_dynamic(
            ctx, static_cast<float>(dflt::SINGLE_ROW_HEIGHT * prog.window_res_mult), 1);

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
            nk_layout_row_dynamic(
                ctx, static_cast<float>((dflt::SINGLE_ROW_HEIGHT * prog.window_res_mult) / 1.5), 1);
            nk_text_colored(ctx, "Airport not found", 17, NK_TEXT_CENTERED,
                nk_color{.r = 255, .g = 0, .b = 0, .a = 255});
        }
    }

    nk_end(ctx);
}
}  // namespace graphics