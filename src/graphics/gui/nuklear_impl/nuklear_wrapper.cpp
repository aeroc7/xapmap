/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "nuklear_wrapper.h"

#include <utils/cairo/rounded_rectangle.h>
#include <utils/utils.h>

#include <cmath>
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

void cairo_nk_triangle_base(cairo_t *cr, double x1, double y1, double x2, double y2, double x3,
    double y3, CairoColor col) noexcept {
    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);
    cairo_new_sub_path(cr);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_line_to(cr, x3, y3);
    cairo_line_to(cr, x1, y1);
    cairo_close_path(cr);
}

void cairo_nk_stroked_triangle(cairo_t *cr, double x1, double y1, double x2, double y2, double x3,
    double y3, double thickness, CairoColor col) noexcept {
    cairo_save(cr);
    cairo_set_line_width(cr, thickness);
    cairo_nk_triangle_base(cr, x1, y1, x2, y2, x3, y3, col);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void cairo_nk_filled_triangle(cairo_t *cr, double x1, double y1, double x2, double y2, double x3,
    double y3, CairoColor col) noexcept {
    cairo_save(cr);
    cairo_nk_triangle_base(cr, x1, y1, x2, y2, x3, y3, col);
    cairo_fill(cr);
    cairo_restore(cr);
}

void cairo_nk_polygon_base(
    cairo_t *cr, const struct nk_vec2i *points, std::size_t points_sz, CairoColor col) noexcept {
    ASSERT(points_sz > 0);
    ASSERT(points != nullptr);

    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);
    cairo_new_sub_path(cr);
    cairo_move_to(cr, points[0].x, points[0].y);

    for (std::size_t i = 1; i < points_sz; ++i) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }

    cairo_line_to(cr, points[0].x, points[0].y);
    cairo_close_path(cr);
}

void cairo_nk_stroked_polygon(cairo_t *cr, const struct nk_vec2i *points, std::size_t points_sz,
    double thickness, CairoColor col) noexcept {
    cairo_save(cr);
    cairo_set_line_width(cr, thickness);
    cairo_nk_polygon_base(cr, points, points_sz, col);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void cairo_nk_filled_polygon(
    cairo_t *cr, const struct nk_vec2i *points, std::size_t points_sz, CairoColor col) noexcept {
    cairo_save(cr);
    cairo_nk_polygon_base(cr, points, points_sz, col);
    cairo_fill(cr);
    cairo_restore(cr);
}

void cairo_nk_text(cairo_t *cr, utils::CairoFont &font, double x, double y, double sz,
    const char *text, CairoColor col) noexcept {
    cairo_save(cr);
    const auto [r, g, b, a] = col.get_rgb();
    cairo_set_source_rgba(cr, r, g, b, a);
    font.set_font_face(cr);

    cairo_text_extents_t te;
    cairo_text_extents(cr, text, &te);
    cairo_set_font_size(cr, sz);
    cairo_move_to(
        cr, x, y + te.height + std::ceil((sz - te.height) / 2.0) - (te.height - -te.y_bearing));
    cairo_show_text(cr, text);
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

}  // namespace

NkGui::NkGui(const xapmap::CurState &prog, ImplCallback style_set, ImplCallback cb) {
    font_stuff.last_state = &prog;
    gui_cb = cb;
    nk_font.userdata = nk_handle_ptr(&font_stuff);
    nk_font.height = font_stuff.FONT_SIZE;
    nk_font.width = [](nk_handle hdl, float, const char *text, int len) -> float {
        cairo_text_extents_t te;
        std::string text_nul_term{text, static_cast<std::size_t>(len)};

        auto fnt_stuff_ref = reinterpret_cast<FontStuffForNk *>(hdl.ptr);
        cairo_set_font_size(fnt_stuff_ref->last_state->cr, fnt_stuff_ref->FONT_SIZE);
        cairo_text_extents(fnt_stuff_ref->last_state->cr, text_nul_term.c_str(), &te);
        return static_cast<float>(te.width + (te.x_bearing * 4.0));
    };

    nk_init_default(&ctx, &nk_font);

    // User-provided callback to set custom styling for Nuklear
    if (style_set) {
        xapmap::CurState::input_event_q_value_type vs;
        style_set(prog, &ctx, vs);
    }
}

void NkGui::set_clear_col(nk_color col) noexcept {
    clear_col = col;
}

void NkGui::draw_frame(const xapmap::CurState &prog) {
    font_stuff.last_state = &prog;
    cairo_clear_surface(prog.cr, clear_col);

    nk_input_begin(&ctx);

    // psa: nk_input_button 0 for mouse is up, 1 for mouse is down
    xapmap::CurState::input_event_q_value_type not_nk_inputs;
    for (auto cursor_event = prog.get_cursor_event(); std::get<bool>(cursor_event);
         cursor_event = prog.get_cursor_event()) {
        const auto event_data = std::get<graphics::CursorStats>(cursor_event);

        if (input_event_is_for_nk(event_data) == false) {
            not_nk_inputs.push(event_data);
            continue;
        }

        switch (event_data.type) {
            case graphics::CursorStatType::MOUSE_MOVE: {
                if (event_data.x_pos > static_cast<std::uint_fast32_t>(prog.window_width) ||
                    event_data.y_pos > static_cast<std::uint_fast32_t>(prog.window_height)) {
                    break;
                }

                nk_input_motion(
                    &ctx, static_cast<int>(event_data.x_pos), static_cast<int>(event_data.y_pos));
                break;
            }
            case graphics::CursorStatType::LEFT_MOUSE_PRESS: {
                nk_input_button(&ctx, NK_BUTTON_LEFT, static_cast<int>(event_data.x_pos),
                    static_cast<int>(event_data.y_pos), 1);
                break;
            }
            case graphics::CursorStatType::LEFT_MOUSE_RELEASE: {
                nk_input_button(&ctx, NK_BUTTON_LEFT, static_cast<int>(event_data.x_pos),
                    static_cast<int>(event_data.y_pos), 0);
                break;
            }
        }
    }

    nk_input_end(&ctx);

    if (gui_cb) {
        gui_cb(prog, &ctx, not_nk_inputs);
    }

    const nk_command *cmd = nullptr;
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
            case NK_COMMAND_TRIANGLE: {
                const auto t = reinterpret_cast<const nk_command_triangle *>(cmd);
                cairo_nk_stroked_triangle(prog.cr, t->a.x, t->a.y, t->b.x, t->b.y, t->c.x, t->c.y,
                    t->line_thickness, t->color);
                break;
            }
            case NK_COMMAND_TRIANGLE_FILLED: {
                const auto t = reinterpret_cast<const nk_command_triangle_filled *>(cmd);
                cairo_nk_filled_triangle(
                    prog.cr, t->a.x, t->a.y, t->b.x, t->b.y, t->c.x, t->c.y, t->color);
                break;
            }
            case NK_COMMAND_POLYGON: {
                const auto p = reinterpret_cast<const nk_command_polygon *>(cmd);
                cairo_nk_stroked_polygon(
                    prog.cr, p->points, p->point_count, p->line_thickness, p->color);
                break;
            }
            case NK_COMMAND_POLYGON_FILLED: {
                const auto p = reinterpret_cast<const nk_command_polygon_filled *>(cmd);
                cairo_nk_filled_polygon(prog.cr, p->points, p->point_count, p->color);
                break;
            }
            case NK_COMMAND_TEXT: {
                const auto t = reinterpret_cast<const nk_command_text *>(cmd);
                auto user_font = reinterpret_cast<utils::CairoFont *>(t->font->userdata.ptr);
                std::string text_nul_term{t->string, static_cast<std::size_t>(t->length)};
                cairo_nk_text(prog.cr, *user_font, t->x, t->y, font_stuff.FONT_SIZE,
                    text_nul_term.c_str(), t->foreground);
                break;
            }
            default:
                break;
        }
    }

    nk_clear(&ctx);
}

bool NkGui::input_event_is_for_nk(const graphics::CursorStats &in_stats) noexcept {
    auto window = ctx.begin;

    while (window) {
        switch (in_stats.type) {
            case graphics::CursorStatType::LEFT_MOUSE_PRESS:
            case graphics::CursorStatType::LEFT_MOUSE_RELEASE:
            case graphics::CursorStatType::MOUSE_MOVE: {
                const struct nk_rect *window_dims = &window->bounds;
                if (NK_INBOX(static_cast<float>(in_stats.x_pos), static_cast<float>(in_stats.y_pos),
                        window_dims->x, window_dims->y, window_dims->w, window_dims->h)) {
                    return true;
                }
                break;
            }
        }

        window = window->next;
    }

    return false;
}

NkGui::~NkGui() {
    nk_free(&ctx);
}
}  // namespace nk_impl