/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "xapmap.h"

#include <config/defaults.h>

#include "graphics/cairo_mt.h"
#include "graphics/window.h"

namespace xapmap {
Xapmap::Xapmap() {
    graphics::Window whdlr;

    whdlr.set_input_cb([this](graphics::InputStats cs) {
        prog.add_cursor_event(cs);
    });

    whdlr.create_window("xapmap", dflt::WINDOW_WIDTH, dflt::WINDOW_HEIGHT);

    if (const GLenum err = glewInit(); err != GLEW_OK) {
        throw std::runtime_error(std::string{"Failed to initialize glew: "} +
                                 reinterpret_cast<const char *>(glewGetErrorString(err)));
    }

    const auto actual_window_dims = whdlr.query_true_window_dims();

    cairo_mt = std::make_unique<graphics::CairoMt>(
        std::get<0>(actual_window_dims), std::get<1>(actual_window_dims), dflt::WINDOW_FPS);

    cairo_mt->set_callbacks(
        [this](cairo_t *cr) {
            prog.cr = cr;
            main_gui.on_start(prog);
        },
        [this](cairo_t *cr) {
            prog.cr = cr;
            main_gui.on_draw(prog);
        },
        [this](cairo_t *cr) {
            prog.cr = cr;
            main_gui.on_stop(prog);
        });

    cairo_mt->start_thread();

    whdlr.window_loop([&]() {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        cairo_mt->blit_texture();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        // This communicates the window height with the render thread
        const auto [w, h] = whdlr.get_window_dims();
        // Safe: atomic variables
        prog.window_width = w;
        prog.window_height = h;
        prog.window_res_mult = static_cast<double>(h) / static_cast<double>(dflt::WINDOW_HEIGHT);
    });
}

Xapmap::~Xapmap() {
    cairo_mt->stop_thread();
}
}  // namespace xapmap