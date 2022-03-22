/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NUKLEAR_CAIRO_COLOR_H_
#define NUKLEAR_CAIRO_COLOR_H_

#include <cairo/cairo.h>
#include <utils/hex_to_rgb.h>

#include <tuple>

struct nk_color;

namespace nk_impl {
class RgbColor final {
public:
    constexpr RgbColor(double pr, double pg, double pb, double pa) : r(pr), g(pg), b(pb), a(pa) {
    }

    RgbColor(const nk_color &col);

    constexpr RgbColor(unsigned col) {
        const auto [tr, tg, tb] = utils::hex_to_rgb(col);
        r = tr;
        g = tg;
        b = tb;
        a = 1.0;
    }

    constexpr RgbColor(const RgbColor &) = default;
    constexpr RgbColor(RgbColor &&) = default;
    constexpr RgbColor &operator=(const RgbColor &) = default;
    constexpr RgbColor &operator=(RgbColor &&) = default;

    constexpr auto get_rgb() const noexcept {
        return std::tuple{r, g, b, a};
    }

    void use_with_cairo(cairo_t *cr) const noexcept {
        cairo_set_source_rgba(cr, r, g, b, a);
    }

private:
    double r, g, b, a;
};
}  // namespace nk_impl

#endif /* NUKLEAR_CAIRO_COLOR_H_ */