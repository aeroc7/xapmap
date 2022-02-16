/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CAIRO_MT_H_
#define CAIRO_MT_H_

#include <cairo/cairo.h>

#include <functional>
#include <thread>

#include "gl_pbo.h"
#include "gl_texture.h"

namespace graphics {
class CairoMt final {
public:
    using CallbackType = std::function<void(cairo_t *)>;
    using dims_type = int;

    explicit CairoMt(dims_type w, dims_type h, unsigned fps);
    void set_callbacks(CallbackType start, CallbackType loop, CallbackType stop);
    void stop_thread();
    void blit_texture();
    CairoMt(const CairoMt &) = delete;
    CairoMt(CairoMt &&) = delete;
    CairoMt &operator=(const CairoMt &) = delete;
    CairoMt &operator=(CairoMt &&) = delete;

private:
    void render_loop() noexcept;
    void do_render_sleep(long time_start, long time_end) const noexcept;
    dims_type width, height;
    unsigned int fps_tgt{};
    GlTexture gl_texture{width, height};
    GlPbo gl_pbo{width, height};
    CallbackType start, loop, stop;
    std::thread render_thread;
    std::atomic_bool quit_thread{false};
};
}  // namespace graphics

#endif /* CAIRO_MT_H_ */