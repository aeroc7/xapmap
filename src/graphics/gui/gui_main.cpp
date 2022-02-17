/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gui_main.h"

namespace graphics {
void GuiMain::on_start(const xapmap::CurState &) {
}

void GuiMain::on_draw(const xapmap::CurState &prog) {
    cairo_set_source_rgba(prog.cr, 0.35, 0.45, 0.75, 1.0);
    cairo_rectangle(prog.cr, 0, 0, prog.window_width, prog.window_height);
    cairo_fill(prog.cr);
}

void GuiMain::on_stop(const xapmap::CurState &) {
}
}  // namespace graphics