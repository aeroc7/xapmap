/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CAIRO_SURFACE_H_
#define CAIRO_SURFACE_H_

#include <cairo/cairo.h>

namespace utils {
class CairoSurface final {
public:
    // Copy operators disabled for now, but trivial to implement later
    // on when needed.
    using dims_type = int;
    CairoSurface(dims_type w, dims_type h);
    CairoSurface(const CairoSurface &) = delete;
    CairoSurface(CairoSurface &&) noexcept;
    CairoSurface &operator=(const CairoSurface &) = delete;
    CairoSurface &operator=(CairoSurface &&) noexcept;

    explicit operator bool() noexcept {
        return surface_good();
    }

    cairo_surface_t *get_surface() noexcept {
        return surface;
    }

    cairo_t *get_cr() noexcept;

    ~CairoSurface();

private:
    bool surface_good() noexcept;
    static constexpr auto SURFACE_FORMAT = CAIRO_FORMAT_ARGB32;

    cairo_t *cr{nullptr};
    cairo_surface_t *surface{nullptr};
};
}  // namespace utils

#endif /* CAIRO_SURFACE_H_ */