/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "apt_dat.h"

namespace xapmap::parsers {
void AptDat::on_line(const std::string &line) noexcept {
    if (line.length() == 0 || line[0] == '#') {
        return;
    }

    const long row_code = rowcode(line);

    switch (row_code) {
        case 1:
            // Use the '1' (land airport) row code to both stop and start an airport entry
            airport_stop();
            airport_start(line);
            break;
        case 1302:

            break;
    }

    first_pass_complete = true;
}

long AptDat::rowcode(const std::string &line) noexcept {
    const std::string row_code = split_by_space(line, 0);
    return i_from_chars<long>(row_code);
}

void AptDat::airport_start(const std::string &line) {
    ap_data.cur_apt = AptDatData();
    ap_data.cur_apt.icao = split_by_space(line, 4);
    ap_data.valid_apt = true;
}

void AptDat::airport_stop() {
    if (first_pass_complete) {
        ap_data.apts[ap_data.cur_apt.icao] = ap_data.cur_apt;
    }
}

void AptDat::airport_datum(const std::string &line) {
    if (!is_valid_apt()) {
        return;
    }

    const std::string data_code = split_by_space(line, 1);

    if (data_code == "city") {
        ap_data.cur_apt.city = split_after_space(line, 2);
    } else if (data_code == "country") {
        ap_data.cur_apt.country = split_after_space(line, 2);
    } else if (data_code == "state") {
        ap_data.cur_apt.state = split_after_space(line, 2);
    } else if (data_code == "datum_lat") {
        ap_data.cur_apt.lat = i_from_chars<double>(split_after_space(line, 2));
    } else if (data_code == "datum_lon") {
        ap_data.cur_apt.lon = i_from_chars<double>(split_after_space(line, 2));
    }
}
}  // namespace xapmap::parsers