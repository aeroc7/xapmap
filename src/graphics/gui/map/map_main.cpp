/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "map_main.h"

namespace graphics {
void MapMain::handle_input_event(const graphics::CursorStats &i) {
    input_stats = i;
}
void MapMain::draw_map(const xapmap::CurState &) {
    // if (parse_hdlr.ap_database.finished()) {
    //     const auto val = parse_hdlr.ap_database.get_task().value();
    //     std::cout << val->get_icao_info("KSEA").value()->name << '\n';
    // }
}
}  // namespace graphics