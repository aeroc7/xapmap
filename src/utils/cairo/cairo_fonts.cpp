/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "cairo_fonts.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "cairo_surface.h"

namespace utils {
static void handle_freetype_error(const FT_Error &e) {
    if (e) {
        throw std::runtime_error(
            "Freetype error occured (code " + std::to_string(static_cast<unsigned>(e)) + ")");
    }
}

CairoFont::CairoFont(const std::vector<std::byte> &fdata) {
    std::copy(fdata.begin(), fdata.end(), std::back_inserter(font_buf));
    create_font();
}

CairoFont::CairoFont(const std::byte *fdata, const std::size_t size) {
    if (size == 0) {
        throw std::invalid_argument("Size can't be zero");
    }

    std::copy(fdata, fdata + static_cast<long>(size), std::back_inserter(font_buf));
    create_font();
}

CairoFont::CairoFont(CairoFont &&o) noexcept
    : font_buf{std::move(o.font_buf)},
      cairo_font_face{std::exchange(o.cairo_font_face, nullptr)},
      ft_lib{std::exchange(o.ft_lib, nullptr)},
      ft_font_face{std::exchange(o.ft_font_face, nullptr)} {
}

CairoFont &CairoFont::operator=(CairoFont &&o) noexcept {
    if (this == &o) {
        return *this;
    }

    font_buf = std::move(o.font_buf);
    cairo_font_face = std::exchange(o.cairo_font_face, nullptr);
    ft_lib = std::exchange(o.ft_lib, nullptr);
    ft_font_face = std::exchange(o.ft_font_face, nullptr);

    return *this;
}

void CairoFont::set_font_face(cairo_t *cr) {
    cairo_set_font_face(cr, cairo_font_face);
}

void CairoFont::create_font() {
    if (font_buf.empty()) {
        throw std::runtime_error("Can't create font from empty buffer");
    }

    auto error = FT_Init_FreeType(&ft_lib);
    handle_freetype_error(error);

    static_assert(std::is_same_v<unsigned char, FT_Byte>);
    error = FT_New_Memory_Face(ft_lib, reinterpret_cast<const FT_Byte *>(font_buf.data()),
        static_cast<long>(font_buf.size()), 0, &ft_font_face);
    handle_freetype_error(error);
}

void CairoFont::create_cairo_font() {
    cairo_font_face = cairo_ft_font_face_create_for_ft_face(ft_font_face, 0);
    const auto status = cairo_font_face_status(cairo_font_face);
    check_cairo_error(status);
}

CairoFont::~CairoFont() {
    FT_Done_Face(ft_font_face);
    FT_Done_FreeType(ft_lib);

    ft_font_face = nullptr;
    ft_lib = nullptr;
}
}  // namespace utils