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

    whdlr.set_cursor_cb([this](graphics::CursorStats cs) {
        prog.add_cursor_event(cs);
    });

    whdlr.create_window("xapmap", dflt::DEFAULT_WINDOW_WIDTH, dflt::DEFAULT_WINDOW_HEIGHT);

    if (const GLenum err = glewInit(); err != GLEW_OK) {
        throw std::runtime_error(std::string{"Failed to initialize glew: "} +
                                 reinterpret_cast<const char *>(glewGetErrorString(err)));
    }

    cairo_mt = std::make_unique<graphics::CairoMt>(
        dflt::DEFAULT_WINDOW_WIDTH, dflt::DEFAULT_WINDOW_HEIGHT, dflt::DEFAULT_WINDOW_FPS);

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

        /*const auto cursor_stats = whdlr.get_mouse_stats();
        prog.cursor.x_pos = cursor_stats.x_pos;
        prog.cursor.y_pos = cursor_stats.y_pos;
        prog.cursor.click = cursor_stats.click;*/
    });
}

Xapmap::~Xapmap() {
    cairo_mt->stop_thread();
}
}  // namespace xapmap