/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MAP_MAIN_H_
#define MAP_MAIN_H_

#include <prog_state.h>

namespace graphics {
class MapMain {
public:
    void handle_input_event(const graphics::CursorStats &i);
    void draw_map(const xapmap::CurState &prog);

private:
    graphics::CursorStats input_stats;
};
}  // namespace graphics

#endif /* MAP_MAIN_H_ */