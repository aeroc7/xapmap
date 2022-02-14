/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "xapmap.h"

namespace xapmap {
Xapmap::Xapmap() {
    whdlr.create_window("xapmap", 750, 500);
    whdlr.window_loop([]() {
    });
}
}  // namespace xapmap