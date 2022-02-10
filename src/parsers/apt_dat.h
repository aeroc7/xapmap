/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef APT_DAT_H_
#define APT_DAT_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace parsers {
struct CoordPair {
    double lat;
    double lon;
};

struct AirportData {
    std::string icao;
    std::string name;
    std::string city;
    std::string state;
    CoordPair coords;
};

struct LrCbParam {
    const std::string &line;
    std::size_t line_num;
    const std::string &filename;
};

class ParseAptDat {
public:
    ParseAptDat(const std::string &path);

private:
    std::vector<std::string> scenery_directories(const std::string &path) const;
    void parse_apt_dat_file(const std::string &e);

    void cur_apt_start(const LrCbParam &);
    void cur_apt_end(const LrCbParam &);
    void cur_apt_1302(const LrCbParam &);

    std::unordered_map<std::string, AirportData> airport_db;

    AirportData cur_apt;
};
}  // namespace parsers

#endif /* APT_DAT_H_ */