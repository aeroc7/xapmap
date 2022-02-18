/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "nuklear_wrapper.h"

#include <utils/cairo/rounded_rectangle.h>
#include <utils/log.h>

#include <string>
#include <tuple>

namespace nk_impl {
class CairoColor {
public:
    CairoColor(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {
    }

    CairoColor(const nk_color &col) {
        r = static_cast<double>(col.r) / 255.0;
        g = static_cast<double>(col.g) / 255.0;
        b = static_cast<double>(col.b) / 255.0;
        a = static_cast<double>(col.a) / 255.0;
    }

    CairoColor(const CairoColor &) = default;
    CairoColor(CairoColor &&) = default;
    CairoColor &operator=(const CairoColor &) = default;
    CairoColor &operator=(CairoColor &&) = default;

    auto get_rgb() const noexcept {
        return std::tuple{r, g, b, a};
    }

private:
    double r, g, b, a;
};
namespace {
void cairo_clear_surface(cairo_t *cr, CairoColor col) noexcept {
    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);
    cairo_paint(cr);
}

void cairo_nk_rectangle_base(
    cairo_t *cr, double x, double y, double w, double h, double rounding, CairoColor col) noexcept {
    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);

    if (rounding > 0.0) {
        utils::rounded_rectangle(cr, x, y, w, h, rounding);
    } else {
        cairo_rectangle(cr, x, y, w, h);
    }
}

void cairo_nk_stroked_rectangle(cairo_t *cr, double x, double y, double w, double h,
    double rounding, double line_thickness, CairoColor col) noexcept {
    cairo_save(cr);
    cairo_nk_rectangle_base(cr, x, y, w, h, rounding, col);
    cairo_set_line_width(cr, line_thickness);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void cairo_nk_filled_rectangle(
    cairo_t *cr, double x, double y, double w, double h, double rounding, CairoColor col) noexcept {
    cairo_save(cr);
    cairo_nk_rectangle_base(cr, x, y, w, h, rounding, col);
    cairo_fill(cr);
    cairo_restore(cr);
}

void cairo_nk_line(cairo_t *cr, double x1, double y1, double x2, double y2, double thickness,
    CairoColor col) noexcept {
    cairo_save(cr);
    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_set_line_width(cr, thickness);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void cairo_nk_circle_base(cairo_t *cr, double x, double y, double radius, CairoColor col) noexcept {
    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);
    cairo_new_sub_path(cr);
    cairo_arc(cr, x, y, radius, 0.0, 2 * M_PI);
    cairo_close_path(cr);
}

void cairo_nk_stroked_circle(cairo_t *cr, double x, double y, double w, double h, double thickness,
    CairoColor col) noexcept {
    double r = (w < h ? w : h) / 2.0;
    cairo_save(cr);
    cairo_nk_circle_base(cr, x + r, y + r, r, col);
    cairo_set_line_width(cr, thickness);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void cairo_nk_filled_circle(
    cairo_t *cr, double x, double y, double w, double h, CairoColor col) noexcept {
    double r = (w < h ? w : h) / 2.0;
    cairo_save(cr);
    cairo_nk_circle_base(cr, x + r, y + r, r, col);
    cairo_fill(cr);
    cairo_restore(cr);
}

void cairo_nk_scissor(cairo_t *cr, double x, double y, double w, double h) noexcept {
    cairo_reset_clip(cr);
    cairo_new_path(cr);
    cairo_move_to(cr, x, y);
    cairo_line_to(cr, x + w, y);
    cairo_line_to(cr, x + w, y + h);
    cairo_line_to(cr, x, y + h);
    cairo_line_to(cr, x, y);
    cairo_clip(cr);
}

nk_user_font font;

}  // namespace
NkGui::NkGui() {
    font.userdata = nk_handle_ptr(nullptr);
    font.height = 13.0f;
    font.width = [](nk_handle, float, const char *, int) {
        return 3.0f;
    };
    nk_init_default(&ctx, &font);
}

enum { EASY, HARD };
static int op = EASY;
static float value = 0.6f;

void NkGui::draw_frame(const xapmap::CurState &prog) {
    cairo_clear_surface(prog.cr, nk_color{1, 0, 0, 255});

    if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
        // fixed widget pixel width
        nk_layout_row_static(&ctx, 30, 80, 1);
        if (nk_button_label(&ctx, "button")) {
            // event handling
        }
        // fixed widget window ratio width
        nk_layout_row_dynamic(&ctx, 30, 2);
        if (nk_option_label(&ctx, "easy", op == EASY))
            op = EASY;
        if (nk_option_label(&ctx, "hard", op == HARD))
            op = HARD;
        // custom widget pixel width
        nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(&ctx, 50);
            nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
            nk_layout_row_push(&ctx, 110);
            nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
        }
        nk_layout_row_end(&ctx);
    }

    const nk_command *cmd;

    nk_foreach(cmd, &ctx) {
        switch (cmd->type) {
            case NK_COMMAND_NOP:
                break;
            case NK_COMMAND_SCISSOR: {
                const auto s = reinterpret_cast<const nk_command_scissor *>(cmd);
                cairo_nk_scissor(prog.cr, s->x, s->y, s->w, s->h);
                break;
            }
            case NK_COMMAND_LINE: {
                const auto l = reinterpret_cast<const nk_command_line *>(cmd);
                cairo_nk_line(prog.cr, l->begin.x, l->begin.y, l->end.x, l->end.y,
                    l->line_thickness, l->color);
                break;
            }
            case NK_COMMAND_RECT: {
                const auto r = reinterpret_cast<const nk_command_rect *>(cmd);
                cairo_nk_stroked_rectangle(
                    prog.cr, r->x, r->y, r->w, r->h, r->rounding, r->line_thickness, r->color);
                break;
            }
            case NK_COMMAND_RECT_FILLED: {
                const auto r = reinterpret_cast<const nk_command_rect_filled *>(cmd);
                cairo_nk_filled_rectangle(prog.cr, r->x, r->y, r->w, r->h, r->rounding, r->color);
                break;
            }
            case NK_COMMAND_CIRCLE: {
                const auto c = reinterpret_cast<const nk_command_circle *>(cmd);
                cairo_nk_stroked_circle(
                    prog.cr, c->x, c->y, c->w, c->h, c->line_thickness, c->color);
                break;
            }
            case NK_COMMAND_CIRCLE_FILLED: {
                const auto c = reinterpret_cast<const nk_command_circle_filled *>(cmd);
                cairo_nk_filled_circle(prog.cr, c->x, c->y, c->w, c->h, c->color);
                break;
            }
            default:
                break;
        }
    }

    nk_clear(&ctx);
    nk_end(&ctx);
}

NkGui::~NkGui() {
    nk_free(&ctx);
}
}  // namespace nk_impl