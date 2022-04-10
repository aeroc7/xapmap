/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "parse_load.h"

#include <graphics/gui/nuklear_impl/nuklear_include.h>

#include <string_view>

constexpr std::string_view window_msg{"Generating airport database... "};

namespace graphics {
void ParseLoad::draw(const xapmap::CurState &prog, nk_context *ctx) {
    if (nk_begin(ctx, "Parse Status",
            nk_recti(prog.window_width / 4, prog.window_height / 4, prog.window_width / 2,
                prog.window_height / 2),
            NK_WINDOW_BORDER | NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, static_cast<float>(prog.window_width) / 5.0f, 1);
        nk_text(ctx, window_msg.data(), window_msg.length(),
            NK_TEXT_ALIGN_MIDDLE | NK_TEXT_ALIGN_CENTERED);
    }

    nk_end(ctx);
}
}  // namespace graphics