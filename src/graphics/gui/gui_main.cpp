/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gui_main.h"

using namespace nk_impl;

namespace graphics {
void GuiMain::on_start(const xapmap::CurState &prog) {
    gui = std::make_unique<NkGui>(prog, [this](nk_context *ctx) {
        gui_callback(ctx);
    });
}

void GuiMain::gui_callback(nk_context *ctx) {
    if (nk_begin(ctx, "MyWindow", nk_recti(10, 10, 210, dflt::DEFAULT_WINDOW_HEIGHT - 20),
            NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE)) {
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "Button!")) {
        }
    }

    nk_end(ctx);
}

void GuiMain::on_draw(const xapmap::CurState &prog) {
    gui->draw_frame(prog);
}

void GuiMain::on_stop(const xapmap::CurState &) {
    gui.reset();
}
}  // namespace graphics