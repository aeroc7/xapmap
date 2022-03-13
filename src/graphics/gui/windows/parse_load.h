/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSE_LOAD_H_
#define PARSE_LOAD_H_

#include <prog_state.h>

struct nk_context;

namespace graphics {
class ParseLoad final {
public:
    void draw(const xapmap::CurState &prog, nk_context *ctx);

private:
};
}  // namespace graphics

#endif /* PARSE_LOAD_H_ */