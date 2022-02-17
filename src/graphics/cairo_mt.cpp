/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "cairo_mt.h"

#include <utils/cairo/cairo_surface.h>
#include <utils/utils.h>

#include <chrono>
#include <cstring>

using namespace utils;

namespace graphics {
CairoMt::CairoMt(dims_type w, dims_type h, unsigned fps) : width(w), height(h), fps_tgt(fps) {
    render_thread = std::thread{&CairoMt::render_loop, this};
}

void CairoMt::do_render_sleep(long time_start, long time_end) const noexcept {
    ASSERT(time_end >= time_start);

    if (time_end == time_start) {
        time_end += 1;
    }

    int64_t diff_time_micros = (time_end - time_start) / 1000;
    const auto ms_sleep_time = static_cast<int64_t>((1000.0 / fps_tgt) * 1000.0);

    if (diff_time_micros >= ms_sleep_time || diff_time_micros < 0) {
        diff_time_micros = 0;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(ms_sleep_time - diff_time_micros));
}

void CairoMt::render_loop() noexcept {
    try {
        CairoSurface cairo_surf{width, height};
        cairo_t *cr = cairo_surf.get_cr();
        cairo_surface_t *surface = cairo_surf.get_surface();

        auto tp_convert = [](auto time_point) {
            const auto tp_us = std::chrono::time_point_cast<std::chrono::microseconds>(time_point)
                                   .time_since_epoch();
            return std::chrono::duration_cast<std::chrono::microseconds>(tp_us).count();
        };

        if (start) {
            start(cr);
        }

        while (!quit_thread) {
            const auto t1 = std::chrono::steady_clock::now();

            if (loop) {
                loop(cr);
            }

            // Finish any operations on surface
            cairo_surface_flush(surface);

            const auto stride = cairo_image_surface_get_stride(surface);
            void *surface_data = cairo_image_surface_get_data(surface);

            gl_pbo.set_buffer_data([&](void *pbo) {
                std::memcpy(pbo, surface_data, static_cast<std::size_t>(stride * this->height));
            });

            const auto t2 = std::chrono::steady_clock::now();
            do_render_sleep(tp_convert(t1), tp_convert(t2));
        }

        if (stop) {
            stop(cr);
        }
    } catch (const std::exception &e) {
        Log(Log::ERROR) << "Caught exception: " << e.what();
    } catch (...) {
        Log(Log::ERROR) << "Caught unknown exception...";
    }
}

void CairoMt::blit_texture() {
    gl_texture.bind();

    gl_pbo.bind_front_buffer();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(0, 0);
    glTexCoord2f(0, 1);
    glVertex2i(0, height);
    glTexCoord2f(1, 1);
    glVertex2i(width, height);
    glTexCoord2f(1, 0);
    glVertex2i(width, 0);
    glEnd();

    gl_texture.unbind();
}

void CairoMt::set_callbacks(CallbackType pstart, CallbackType ploop, CallbackType pstop) {
    start = pstart;
    loop = ploop;
    stop = pstop;
}

void CairoMt::stop_thread() {
    quit_thread = true;
    render_thread.join();
}
}  // namespace graphics