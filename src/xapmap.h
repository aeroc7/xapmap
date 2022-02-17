/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef XAPMAP_H_
#define XAPMAP_H_

#include <memory>

#include "graphics/gui/gui_main.h"
#include "prog_state.h"

namespace graphics {
class CairoMt;
}

namespace xapmap {
class Xapmap final {
public:
    Xapmap();
    ~Xapmap();

private:
    CurState prog;
    std::unique_ptr<graphics::CairoMt> cairo_mt;
    graphics::GuiMain main_gui;
};
}  // namespace xapmap

#endif /* XAPMAP_H_ */