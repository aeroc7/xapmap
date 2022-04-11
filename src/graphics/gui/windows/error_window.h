/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ERROR_WINDOW_H_
#define ERROR_WINDOW_H_

#include <prog_state.h>

#include <string>

struct nk_context;

namespace graphics {
class ErrorWindowBegin final {
public:
    static constexpr auto WIDTH = dflt::SINGLE_ROW_HEIGHT * 8;
    static constexpr auto HEIGHT = dflt::SINGLE_ROW_HEIGHT * 3;
    static constexpr auto NAME = "Error";

    ErrorWindowBegin(const xapmap::CurState &prog, nk_context *ctx, const std::string &error_msg);
    void show(nk_context *ctx) const noexcept;

private:
};
}  // namespace graphics

#endif /* ERROR_WINDOW_H_ */