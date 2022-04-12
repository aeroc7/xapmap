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
    const auto w_height = static_cast<int>(dflt::SINGLE_ROW_HEIGHT * prog.window_res_mult) * 4;
    if (nk_begin(ctx, NAME,
            nk_recti(
                prog.window_width / 3, prog.window_height / 4, prog.window_width / 3, w_height),
            NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, static_cast<float>(w_height) / 2.0f, 1);
        nk_text_colored(ctx, error_msg.c_str(), 17, NK_TEXT_CENTERED,
            nk_color{.r = 255, .g = 0, .b = 0, .a = 255});
    }

    nk_end(ctx);
}

void ErrorWindowBegin::show(nk_context *ctx) const noexcept {
    nk_window_show(ctx, NAME, NK_SHOWN);
}

}  // namespace graphics