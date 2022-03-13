/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MAP_CONF_UI_H_
#define MAP_CONF_UI_H_

#include <parsers/parse_hdlr.h>
#include <utils/log.h>

#include <string>

namespace xapmap {
class MapConfUi final {
public:
    bool ap_db_set_if_exists(const std::string &icao) const {
        if (!parse_hdlr.ap_database.finished()) {
            return false;
        }

        const auto ap_db = parse_hdlr.ap_database.get_task().value();
        const auto ap_info = ap_db->get_icao_info(icao);

        if (!ap_info.has_value()) {
            return false;
        }

        // Set airport map state

        return true;
    }

private:
    parsers::ParseHdlr parse_hdlr;
};
}  // namespace xapmap

#endif /* MAP_CONF_UI_H_ */