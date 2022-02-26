/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gui_main.h"

using namespace nk_impl;

namespace graphics {
void GuiMain::on_start(const xapmap::CurState &prog) {
    gui = std::make_unique<NkGui>(prog);
}

void GuiMain::on_draw(const xapmap::CurState &prog) {
    gui->draw_frame(prog);
}

void GuiMain::on_stop(const xapmap::CurState &) {
    gui.reset();
}
}  // namespace graphics