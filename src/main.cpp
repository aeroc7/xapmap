/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <utils/log.h>

#include <cstdlib>

#include "xapmap.h"

using namespace xapmap;
using namespace utils;

namespace {
std::unique_ptr<Xapmap> xap_map;
}

int main() {
    try {
        xap_map = std::make_unique<Xapmap>();
    } catch (const std::exception &e) {
        Log(Log::ERROR) << "Caught exception: " << e.what();
    } catch (...) {
        Log(Log::ERROR) << "Caught unknown exception";
    }

    xap_map.reset();

    return EXIT_SUCCESS;
}