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
        [](const xapmap::CurState &, nk_context *ctx,
            xapmap::CurState::input_event_q_value_type &) {
            set_gui_theme(ctx);
        },
        [this](const xapmap::CurState &prog, nk_context *ctx,
            xapmap::CurState::input_event_q_value_type &e) {
            gui_callback(prog, ctx, e);
        });

    gui->set_clear_col({35, 35, 35, 255});

    map = std::make_unique<graphics::MapMain>();
}

void GuiMain::gui_callback(const xapmap::CurState &prog, nk_context *ctx,
    xapmap::CurState::input_event_q_value_type &input_events) {
    while (!input_events.empty()) {
        const auto &ev = input_events.front();
        map->handle_input_event(ev);
        input_events.pop();
    }

    map->draw_map(prog);

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
    map.reset();
    gui.reset();
}
}  // namespace graphics