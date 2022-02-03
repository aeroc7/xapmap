/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "parser_base.h"

#include <fstream>

constexpr auto APT_DAT_LOC = "/Custom Scenery/Global Airports/Earth nav data/apt.dat";

namespace xapmap::parsers {
void ParserBase::parse(std::string fn) {
    fn += APT_DAT_LOC;
    std::ifstream file(fn);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file " + fn));
    }

    while (std::getline(file, line)) {
        if (stop_parsing) {
            break;
        }

        on_line(line);
    }
}

void ParserBase::cancel() {
    stop_parsing = true;
}
}  // namespace xapmap::parsers