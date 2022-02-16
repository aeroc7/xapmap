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
class CairoSurfaceError : public std::exception {
public:
    CairoSurfaceError(const std::string &msg) : message{ERROR_HEADER + msg} {
    }

    const char *what() const noexcept {
        return message.c_str();
    }

private:
    static constexpr auto ERROR_HEADER = "Cairo surface error: ";
    std::string message;
};

constexpr void check_cairo_surface_error(cairo_status_t status) {
    switch (status) {
        case CAIRO_STATUS_NULL_POINTER:
            throw CairoSurfaceError("NULL_POINTER");
        case CAIRO_STATUS_NO_MEMORY:
            throw CairoSurfaceError("NO_MEMORY");
        case CAIRO_STATUS_READ_ERROR:
            throw CairoSurfaceError("READ_ERROR");
        case CAIRO_STATUS_INVALID_CONTENT:
            throw CairoSurfaceError("INVALID_CONTENT");
        case CAIRO_STATUS_INVALID_FORMAT:
            throw CairoSurfaceError("INVALID_FORMAT");
        case CAIRO_STATUS_INVALID_VISUAL:
            throw CairoSurfaceError("INVALID_VISUAL");
        case CAIRO_STATUS_SUCCESS:
        default:
            break;
    }
}
CairoSurface::CairoSurface(dims_type w, dims_type h) {
    ASSERT(w > 0);
    ASSERT(h > 0);
    surface = cairo_image_surface_create(SURFACE_FORMAT, w, h);
    check_cairo_surface_error(cairo_surface_status(surface));
}

CairoSurface::CairoSurface(CairoSurface &&other) noexcept
    : surface{std::exchange(other.surface, nullptr)} {
}

CairoSurface &CairoSurface::operator=(CairoSurface &&other) noexcept {
    if (this != &other) {
        surface = std::exchange(other.surface, nullptr);
    }

    return *this;
}

bool CairoSurface::surface_good() noexcept {
    if (surface == nullptr) {
        return false;
    }

    return cairo_surface_status(surface) == CAIRO_STATUS_SUCCESS;
}

cairo_t *CairoSurface::get_cr() noexcept {
    cr = cairo_create(surface);
    check_cairo_surface_error(cairo_status(cr));
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