/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "error_window.h"

#include <graphics/gui/nuklear_impl/nuklear_include.h>

namespace graphics {
ErrorWindowBegin::ErrorWindowBegin(
    const xapmap::CurState &prog, nk_context *ctx, const std::string &error_msg) {
    if (nk_begin(ctx, NAME,
            nk_recti(prog.window_width / 4, prog.window_height / 4,
                static_cast<int>(WIDTH * prog.window_res_mult),
                static_cast<int>(HEIGHT * prog.window_res_mult)),
            NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(
            ctx, static_cast<float>(dflt::SINGLE_ROW_HEIGHT * prog.window_res_mult) * 1.5f, 1);
        nk_text_colored(ctx, error_msg.c_str(), 17, NK_TEXT_CENTERED,
            nk_color{.r = 255, .g = 0, .b = 0, .a = 255});
    }

    nk_end(ctx);
}

void ErrorWindowBegin::show(nk_context *ctx) const noexcept {
    nk_window_show(ctx, NAME, NK_SHOWN);
}

}  // namespace graphics