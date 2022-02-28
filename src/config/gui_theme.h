/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUI_THEME_H_
#define GUI_THEME_H_

struct nk_context;

namespace graphics {
void set_gui_theme(nk_context *ctx) noexcept;
}

#endif /* GUI_THEME_H_ */