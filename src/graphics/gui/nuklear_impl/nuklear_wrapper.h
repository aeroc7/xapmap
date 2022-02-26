/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NUKLEAR_IMPL_H_
#define NUKLEAR_IMPL_H_

#include <Roboto_Medium.h>
#include <prog_state.h>
#include <utils/cairo/cairo_fonts.h>

#include "nuklear_include.h"

namespace nk_impl {
class NkGui final {
public:
    NkGui(const xapmap::CurState &prog);
    NkGui(const NkGui &) = delete;
    NkGui(NkGui &&) = delete;
    NkGui &operator=(const NkGui &) = delete;
    NkGui &operator=(NkGui &&) = delete;
    void draw_frame(const xapmap::CurState &prog);
    ~NkGui();

private:
    struct FontStuffForNk {
        utils::CairoFont roboto_fnt{
            reinterpret_cast<const std::byte *>(bin2h::ROBOTO_MEDIUM), bin2h::ROBOTO_MEDIUM_SIZE};
        const xapmap::CurState *last_state;
        static constexpr auto FONT_SIZE = 15.5;
    };

    FontStuffForNk font_stuff;
    nk_user_font nk_font;
    nk_context ctx;
};
}  // namespace nk_impl

#endif /* NUKLEAR_IMPL_H_ */