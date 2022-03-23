/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef APT_DAT_H_
#define APT_DAT_H_

#include <atomic>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace parsers {
struct CoordPair {
    double lat;
    double lon;
};

enum class NodeOpenType : int { AIRPORT_BOUNDARY = 130, NONE = -1 };

// Info for an airport runway. Respective ends have different coords, name.
// Overall width is the same, as it's the same pavement, just opposite ends.
struct AirportRunwayData {
    double width;
    std::array<char, 4> name[2]{};
    CoordPair latitude[2];
    CoordPair longitude[2];
};

struct AirportData {
    std::string icao;
    std::string name;
    std::string city;
    std::string country;
    std::string state;
    CoordPair coords;
    std::vector<AirportRunwayData> runways;
    std::vector<CoordPair> bounds;
};

struct LrCbParam {
    const std::string &line;
    std::size_t line_num;
    const std::string &filename;
};

class ParseAptDat {
public:
    ParseAptDat(const std::string &path, const std::atomic_bool &);
    std::optional<const AirportData *> get_icao_info(const std::string &icao) const;

private:
    std::vector<std::string> scenery_directories(const std::string &path) const;
    void parse_apt_dat_file(const std::string &e);
    void verify_node_zerod(const LrCbParam &) const;
    void clear_cur_node() noexcept {
        cur_node = NodeOpenType::NONE;
    }
    void set_cur_node(NodeOpenType n) noexcept {
        cur_node = n;
    }

    void cur_apt_start(const LrCbParam &);
    void cur_apt_end(const LrCbParam &);
    void cur_apt_1302(const LrCbParam &);
    void cur_apt_100(const LrCbParam &);
    void cur_apt_111(const LrCbParam &);
    void cur_apt_113(const LrCbParam &);
    void cur_apt_114(const LrCbParam &);
    void cur_apt_130(const LrCbParam &);

    CoordPair extract_node(const LrCbParam &);

    std::unordered_map<std::string, AirportData> airport_db;
    const std::atomic_bool &stop_parsing;
    AirportData cur_apt;
    NodeOpenType cur_node;
};
}  // namespace parsers

#endif /* APT_DAT_H_ */