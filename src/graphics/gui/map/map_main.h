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

#include "map_tk/map_perimeter.h"

namespace graphics {
class MapMain {
public:
    void handle_input_event(const graphics::InputStats &i);
    void draw_map(const xapmap::CurState &prog);

private:
    auto get_airport_hdl(const xapmap::CurState &prog, const std::string &icao);
    graphics::InputStats input_stats;
    graphics::MapPerimeter perim;
};
}  // namespace graphics

#endif /* MAP_MAIN_H_ */