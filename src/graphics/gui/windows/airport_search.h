/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef AIRPORT_SEARCH_H_
#define AIRPORT_SEARCH_H_

#include <prog_state.h>

#include <string>

struct nk_context;

namespace graphics {
class AirportSearch final {
public:
    AirportSearch();
    void draw(const xapmap::CurState &prog, nk_context *ctx);

    static constexpr auto WINDOW_NAME = "Airport Lookup";

private:
    std::string input_buf;
    static constexpr std::size_t INPUT_BUF_MAX = 9;
};
}  // namespace graphics

#endif /* AIRPORT_SEARCH_H_ */