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