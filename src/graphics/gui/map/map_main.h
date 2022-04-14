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
#include <utils/on_param_change.h>

#include <tuple>

#include "map_tk/map_perimeter.h"

namespace graphics {
class MapMain {
public:
    MapMain(const xapmap::CurState &prog);
    void handle_input_event(const graphics::InputStats &i);
    void draw_map(const xapmap::CurState &prog);

private:
    auto get_airport_hdl(const xapmap::CurState &prog, const std::string &icao);
    std::array<parsers::CoordPair, 2> ap_find_bounds(
        const std::vector<parsers::CoordPair> &points) const noexcept;
    std::tuple<double, double> ap_calc_bb_dims(
        const std::array<parsers::CoordPair, 2> &max_bounds) const noexcept;

    graphics::InputStats input_stats;
    graphics::MapPerimeter perim;
    utils::OnParamChange<std::string> on_airport_update;
};

}  // namespace graphics

#endif /* MAP_MAIN_H_ */