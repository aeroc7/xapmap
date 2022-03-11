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

    map_gui = std::make_unique<graphics::MapMain>();
    input_buf.reserve(INPUT_BUF_MAX);
}

void GuiMain::gui_callback(const xapmap::CurState &prog, nk_context *ctx,
    xapmap::CurState::input_event_q_value_type &input_events) {
    while (!input_events.empty()) {
        const auto &ev = input_events.front();
        map_gui->handle_input_event(ev);
        input_events.pop();
    }

    map_gui->draw_map(prog);

    if (nk_begin(ctx, "MainWindow", nk_recti(0, 0, 0, 0), NK_WINDOW_BACKGROUND)) {
    }
    nk_end(ctx);

    if (nk_begin(ctx, "SubWindow", nk_recti(50, 50, 250, 350),
            NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MOVABLE | NK_WINDOW_BORDER |
                NK_WINDOW_SCALABLE)) {
        nk_layout_row_dynamic(ctx, 45, 1);
        nk_edit_string_zero_terminated(
            ctx, NK_EDIT_FIELD, input_buf.data(), static_cast<int>(INPUT_BUF_MAX), nk_filter_ascii);
    }

    nk_end(ctx);
}

void GuiMain::on_draw(const xapmap::CurState &prog) {
    gui->draw_frame(prog);
}

void GuiMain::on_stop(const xapmap::CurState &) {
    map_gui.reset();
    gui.reset();
}
}  // namespace graphics