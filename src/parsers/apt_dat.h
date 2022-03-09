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

    void cur_apt_start(const LrCbParam &);
    void cur_apt_end(const LrCbParam &);
    void cur_apt_1302(const LrCbParam &);
    void cur_apt_100(const LrCbParam &);

    std::unordered_map<std::string, AirportData> airport_db;
    const std::atomic_bool &stop_parsing;
    AirportData cur_apt;
};
}  // namespace parsers

#endif /* APT_DAT_H_ */