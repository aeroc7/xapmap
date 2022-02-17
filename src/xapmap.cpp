/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "xapmap.h"

#include <config/defaults.h>

namespace xapmap {
Xapmap::Xapmap() {
    whdlr.create_window("xapmap", dflt::DEFAULT_WINDOW_WIDTH, dflt::DEFAULT_WINDOW_HEIGHT);

    if (const GLenum err = glewInit(); err == GLEW_OK) {
        throw std::runtime_error(std::string{"Failed to initialize glew: "} +
                                 reinterpret_cast<const char *>(glewGetErrorString(err)));
    }

    graphics::CairoMt mt{dflt::DEFAULT_WINDOW_WIDTH, dflt::DEFAULT_WINDOW_HEIGHT, 60};

    mt.set_callbacks(
        [](cairo_t *) {
        },
        [](cairo_t *cr) {
            cairo_set_source_rgb(cr, 0, 1, 0);
            cairo_rectangle(cr, 50, 50, 100, 100);
            cairo_fill(cr);
        },
        [](cairo_t *) {
        });

    whdlr.window_loop([&mt]() {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        mt.blit_texture();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    });

    mt.stop_thread();
}
}  // namespace xapmap