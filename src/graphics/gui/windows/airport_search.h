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

#include <array>

struct nk_context;

namespace graphics {
class AirportSearch final {
public:
    void draw(const xapmap::CurState &prog, nk_context *ctx);

    static constexpr auto WINDOW_NAME = "Airport Lookup";

private:
    static constexpr std::size_t INPUT_BUF_MAX = 9;
    std::array<char, INPUT_BUF_MAX> input_buf{'\0'};
    bool airport_not_found{false};
};
}  // namespace graphics

#endif /* AIRPORT_SEARCH_H_ */