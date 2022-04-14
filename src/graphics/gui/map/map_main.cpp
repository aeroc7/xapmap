/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "map_main.h"

#include <utils/map/haversine.h>

#include <algorithm>

namespace graphics {
MapMain::MapMain(const xapmap::CurState &) {
}

auto MapMain::get_airport_hdl(const xapmap::CurState &prog, const std::string &icao) {
    return prog.map_conf.database().ap_database.get_task().value()->get_icao_info(icao).value();
}

void MapMain::handle_input_event(const graphics::InputStats &i) {
    input_stats = i;
}

void MapMain::draw_map(const xapmap::CurState &prog) {
    if (!prog.map_conf.parsing_has_finished()) {
        return;
    }

    const auto &map_state = prog.map_conf.get_latest_state();
    if (!map_state.tgt_ident.valid()) {
        return;
    }

    const auto airport_hdl = get_airport_hdl(prog, map_state.tgt_ident.get_item());
    // Only runs if the map_state.tgt_ident value changes (whenever the user inputs a new ICAO and
    // it's a valid one)
    on_airport_update(map_state.tgt_ident.get_item(), [&] {
        ASSERT(airport_hdl->bounds.size() > 0);
        const auto bnds = ap_find_bounds(airport_hdl->bounds);
        [[maybe_unused]] const auto [bb_w, bb_h] = ap_calc_bb_dims(bnds);
    });

    perim.draw(prog, *airport_hdl);
}

std::array<parsers::CoordPair, 2> MapMain::ap_find_bounds(
    const std::vector<parsers::CoordPair> &points) const noexcept {
    ASSERT(points.size() >= 2);
    std::array<parsers::CoordPair, 2> bound_box{parsers::CoordPair{points[0].lat, points[0].lon},
        parsers::CoordPair{points[1].lat, points[1].lon}};

    for (const auto &e : points) {
        const auto cur_lat = e.lat;
        const auto cur_lon = e.lon;

        bound_box[0].lat = std::min(bound_box[0].lat, cur_lat);
        bound_box[0].lon = std::min(bound_box[0].lon, cur_lon);

        bound_box[1].lat = std::max(bound_box[1].lat, cur_lat);
        bound_box[1].lon = std::max(bound_box[1].lon, cur_lon);
    }

    return bound_box;
}

std::tuple<double, double> MapMain::ap_calc_bb_dims(
    const std::array<parsers::CoordPair, 2> &max_bounds) const noexcept {
    const auto bb_avg_lat = (max_bounds[0].lat + max_bounds[1].lat) / 2.0;
    const auto bb_avg_lon = (max_bounds[0].lon + max_bounds[1].lon) / 2.0;

    const auto bb_width_m = utils::haversine_formula_meters(
        bb_avg_lat, max_bounds[0].lon, bb_avg_lat, max_bounds[1].lon);
    const auto bb_height_m = utils::haversine_formula_meters(
        max_bounds[0].lat, bb_avg_lon, max_bounds[1].lat, bb_avg_lon);

    return {bb_width_m, bb_height_m};
}
}  // namespace graphics