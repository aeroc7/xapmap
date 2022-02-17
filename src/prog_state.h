/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef STATE_H_
#define STATE_H_

#include <cairo/cairo.h>
#include <config/defaults.h>

#include <atomic>

namespace xapmap {
class CurState {
public:
    cairo_t *cr{nullptr};
    std::atomic<int> window_width{dflt::DEFAULT_WINDOW_WIDTH};
    std::atomic<int> window_height{dflt::DEFAULT_WINDOW_HEIGHT};
};
}  // namespace xapmap

#endif /* STATE_H_ */