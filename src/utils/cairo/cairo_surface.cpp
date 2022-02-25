/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "cairo_surface.h"

#include <utils/utils.h>

#include <algorithm>
#include <string>

namespace utils {
CairoSurface::CairoSurface(dims_type w, dims_type h) {
    ASSERT(w > 0);
    ASSERT(h > 0);
    surface = cairo_image_surface_create(SURFACE_FORMAT, w, h);
    check_cairo_error(cairo_surface_status(surface));

    cr = cairo_create(surface);
    check_cairo_error(cairo_status(cr));
}

CairoSurface::CairoSurface(CairoSurface &&other) noexcept
    : cr{std::exchange(other.cr, nullptr)}, surface{std::exchange(other.surface, nullptr)} {
}

CairoSurface &CairoSurface::operator=(CairoSurface &&other) noexcept {
    if (this != &other) {
        surface = std::exchange(other.surface, nullptr);
        cr = std::exchange(other.cr, nullptr);
    }

    return *this;
}

bool CairoSurface::surface_good() noexcept {
    if (!surface || !cr) {
        return false;
    }

    auto surf_success = [](auto surf) {
        return (surf) == CAIRO_STATUS_SUCCESS;
    };

    return surf_success(cairo_surface_status(surface)) && surf_success(cairo_status(cr));
}

cairo_t *CairoSurface::get_cr() noexcept {
    return cr;
}

CairoSurface::~CairoSurface() {
    if (cr) {
        cairo_destroy(cr);
        cr = nullptr;
    }

    if (surface) {
        cairo_surface_destroy(surface);
        surface = nullptr;
    }
}

}  // namespace utils