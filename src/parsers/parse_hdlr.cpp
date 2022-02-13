/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "parse_hdlr.h"

#include "apt_dat.h"

namespace parsers {
ParseHdlr::ParseHdlr() {
    apt_dat_hdlr = std::make_unique<ParseAptDat>("/home/bennett/X-Plane 11/");
}
}  // namespace parsers