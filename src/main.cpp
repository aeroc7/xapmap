/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdlib>

#include "xapmap.h"

using namespace xapmap;

namespace {
std::unique_ptr<Xapmap> xap_map;
}

int main() {
    xap_map = std::make_unique<Xapmap>();
    xap_map.reset();
    return EXIT_SUCCESS;
}