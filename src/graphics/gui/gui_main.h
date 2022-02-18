/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUI_MAIN_H_
#define GUI_MAIN_H_

#include <prog_state.h>

#include <memory>

#include "nuklear_impl/nuklear_wrapper.h"

namespace graphics {
class GuiMain {
public:
    void on_start(const xapmap::CurState &prog);
    void on_draw(const xapmap::CurState &prog);
    void on_stop(const xapmap::CurState &prog);

private:
    std::unique_ptr<nk_impl::NkGui> gui;
};
}  // namespace graphics

#endif /* GUI_MAIN_H_ */