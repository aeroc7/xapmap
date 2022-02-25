/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CAIRO_FONTS_H_
#define CAIRO_FONTS_H_

#include <cairo/cairo-ft.h>

#include <cstdint>
#include <vector>

namespace utils {
class CairoFont {
public:
    CairoFont(const std::vector<std::byte> &fdata);
    CairoFont(const std::byte *fdata, const std::size_t size);
    CairoFont(CairoFont &&) noexcept;
    CairoFont &operator=(CairoFont &&) noexcept;
    CairoFont(const CairoFont &) = delete;
    CairoFont &operator=(const CairoFont &) = delete;
    void set_font_face(cairo_t *cr);

    ~CairoFont();

private:
    void create_font();
    void create_cairo_font();
    std::vector<std::byte> font_buf;
    cairo_font_face_t *cairo_font_face{};
    FT_Library ft_lib{};
    FT_Face ft_font_face{};
};
}  // namespace utils

#endif /* CAIRO_FONTS_H_ */