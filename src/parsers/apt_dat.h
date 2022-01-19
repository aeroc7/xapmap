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

#include "parser_base.h"

namespace xapmap::parsers {
struct AptDatData {
    std::string icao;
    std::string name;
    std::string city;
    std::string state;
    std::string country;
    double lat{1000}, lon{1000};
};

class AptDat : public ParserBase {
public:
    ~AptDat() = default;

private:
    void on_line(const std::string &line) noexcept override;
    long rowcode(const std::string &line) noexcept;
    void airport_start(const std::string &line);
    void airport_stop();
    void airport_datum(const std::string &line);
    bool is_valid_apt() const noexcept { return ap_data.valid_apt; }

    struct {
        AptDatData cur_apt;
        bool valid_apt{false};
        std::unordered_map<std::string, AptDatData> apts;
    } ap_data;

    bool first_pass_complete{false};
};
}  // namespace xapmap::parsers

#endif /* APT_DAT_H_ */