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
AirportSearch::AirportSearch() {
    input_buf.reserve(INPUT_BUF_MAX);
}

void AirportSearch::draw(const xapmap::CurState &, nk_context *ctx) {
    if (nk_begin(ctx, WINDOW_NAME,
            nk_recti(dflt::DEFAULT_WINDOW_WIDTH / 4, dflt::DEFAULT_WINDOW_HEIGHT / 4,
                dflt::DEFAULT_WINDOW_WIDTH / 2, dflt::DEFAULT_WINDOW_HEIGHT / 2),
            NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_DYNAMIC | NK_WINDOW_MOVABLE |
                NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 35, 1);
        nk_edit_string_zero_terminated(
            ctx, NK_EDIT_FIELD, input_buf.data(), static_cast<int>(INPUT_BUF_MAX), nk_filter_ascii);
        nk_layout_row_dynamic(ctx, 2, 1);
        nk_layout_row_dynamic(ctx, 35, 1);
        nk_button_text(ctx, "Search", 6);
    }

    nk_end(ctx);
}
}  // namespace graphics