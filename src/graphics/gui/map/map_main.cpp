/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "map_main.h"

namespace graphics {
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

    // const auto airport_hdl = get_airport_hdl(prog, map_state.tgt_ident.get_item());
}
}  // namespace graphics