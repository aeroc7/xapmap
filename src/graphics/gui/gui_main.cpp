/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gui_main.h"

#include <config/gui_theme.h>

using namespace nk_impl;

namespace graphics {
void GuiMain::on_start(const xapmap::CurState &prog) {
    gui = std::make_unique<NkGui>(
        prog,
        [](const xapmap::CurState &, nk_context *ctx) {
            set_gui_theme(ctx);
        },
        [this](const xapmap::CurState &prog, nk_context *ctx) {
            gui_callback(prog, ctx);
        });

    gui->set_clear_col({35, 35, 35, 255});
}

void GuiMain::gui_callback(const xapmap::CurState &prog, nk_context *ctx) {
    if (nk_begin(ctx, "MainWindow", nk_recti(0, 0, 0, 0), NK_WINDOW_BACKGROUND)) {
    }
    nk_end(ctx);

    if (nk_begin(ctx, "SubWindow", nk_recti(50, 50, 250, 350),
            NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MOVABLE | NK_WINDOW_BORDER |
                NK_WINDOW_SCALABLE)) {
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