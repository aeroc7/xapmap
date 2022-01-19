/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef XAPMAP_H_
#define XAPMAP_H_

#include <memory>

#include "parsers/parsers.h"

namespace xapmap {
class Xapmap {
public:
    Xapmap();
    ~Xapmap();

private:
    std::unique_ptr<parsers::Parsers> parser_hdlr;
};
}  // namespace xapmap

#endif /* XAPMAP_H_ */