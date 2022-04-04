/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MAP_CONF_STATE_H_
#define MAP_CONF_STATE_H_

#include <parsers/parse_hdlr.h>
#include <utils/log.h>

#include <string>

namespace xapmap {
template <typename T>
class StateMember {
public:
    using ValType = T;

    const T &get_item() const {
        if (!valid()) {
            throw std::runtime_error("State member is not valid");
        }

        return item.value();
    }

    bool valid() const noexcept {
        return item.has_value();
    }

    explicit operator const T &() const noexcept {
        return get_item();
    }

private:
    friend class MapConfUi;
    std::optional<T> item;
};

struct MapState {
    StateMember<std::string> tgt_ident;  // icao
};

class MapConfUi final {
public:
    MapConfUi(const std::string &path) {
        parse_hdlr = std::make_unique<parsers::ParseHdlr>(path);
    }

    bool ap_db_set_if_exists(std::string icao) {
        if (!parse_hdlr->ap_database.finished() || icao.empty()) {
            return false;
        }

        // Ensure that the input is converted to all uppercase
        std::transform(icao.begin(), icao.end(), icao.begin(), [](unsigned char c) {
            return std::toupper(c);
        });

        const auto ap_db = parse_hdlr->ap_database.get_task().value();
        const auto ap_info = ap_db->get_icao_info(icao);

        if (!ap_info.has_value()) {
            return false;
        }

        // Set airport map state
        set_item(cur_state.tgt_ident, icao);

        return true;
    }

    bool parsing_has_finished() const noexcept {
        return parse_hdlr->ap_database.finished();
    }

    MapState &modify_state() noexcept {
        return cur_state;
    }

    const MapState &get_latest_state() const noexcept {
        return cur_state;
    }

    const parsers::ParseHdlr &database() const {
        return *parse_hdlr.get();
    }

private:
    template <typename U>
    void set_item(StateMember<U> &m, const U &val) {
        m.item = std::make_optional<U>(val);
    }

    std::unique_ptr<parsers::ParseHdlr> parse_hdlr;
    mutable MapState cur_state;
};
}  // namespace xapmap

#endif /* MAP_CONF_STATE_H_ */