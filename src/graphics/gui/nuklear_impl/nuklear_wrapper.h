/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NUKLEAR_IMPL_H_
#define NUKLEAR_IMPL_H_

#include <Roboto_Regular.h>
#include <prog_state.h>
#include <utils/cairo/cairo_fonts.h>

#include <functional>

#include "nuklear_include.h"

namespace nk_impl {
class NkGui final {
public:
    using ImplCallback = std::function<void(const xapmap::CurState &prog, nk_context *ctx,
        xapmap::CurState::input_event_q_value_type &events)>;
    NkGui(const xapmap::CurState &prog, ImplCallback style_set, ImplCallback cb);
    NkGui(const NkGui &) = delete;
    NkGui(NkGui &&) = delete;
    NkGui &operator=(const NkGui &) = delete;
    NkGui &operator=(NkGui &&) = delete;
    void draw_frame(const xapmap::CurState &prog);
    void set_clear_col(nk_color col) noexcept;
    ~NkGui();

private:
    struct FontStuffForNk {
        utils::CairoFont roboto_fnt{
            reinterpret_cast<const std::byte *>(bin2h::ROBOTO_REGULAR), bin2h::ROBOTO_REGULAR_SIZE};
        const xapmap::CurState *last_state;
        static constexpr auto FONT_SIZE = 20;
    };

    bool input_event_is_for_nk(const graphics::InputStats &in_stats) noexcept;

    FontStuffForNk font_stuff;
    nk_user_font nk_font;
    nk_context ctx;
    ImplCallback gui_cb;
    nk_color clear_col{0, 0, 0, 255};
};
}  // namespace nk_impl

#endif /* NUKLEAR_IMPL_H_ */