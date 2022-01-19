/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "parsers.h"

#include <utils/log.h>

#include "apt_dat.h"

namespace xapmap::parsers {
Parsers::Parsers() { thread = std::thread(&Parsers::thread_func, this); }

void Parsers::thread_func() {
    try {
        AptDat apt;
        apt.parse(USER_XPLANE_ROOT);
    } catch (const std::exception &e) {
        utils::Log() << "Caught exception: " << e.what();
    }
}

Parsers::~Parsers() { thread.join(); }
}  // namespace xapmap::parsers