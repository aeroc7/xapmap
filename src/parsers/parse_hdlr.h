/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSE_HDLR_H_
#define PARSE_HDLR_H_

#include <memory>

#include "apt_dat.h"

namespace parsers {
class ParseHdlr {
public:
    ParseHdlr();

private:
    std::unique_ptr<ParseAptDat> apt_dat_hdlr;
};
}  // namespace parsers

#endif /* PARSE_HDLR_H_ */