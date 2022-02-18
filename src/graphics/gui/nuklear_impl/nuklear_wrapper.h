/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NUKLEAR_IMPL_H_
#define NUKLEAR_IMPL_H_

#include <prog_state.h>

#include "nuklear_include.h"

namespace nk_impl {
class NkGui final {
public:
    NkGui();
    NkGui(const NkGui &) = delete;
    NkGui(NkGui &&) = delete;
    NkGui &operator=(const NkGui &) = delete;
    NkGui &operator=(NkGui &&) = delete;
    void draw_frame(const xapmap::CurState &prog);
    ~NkGui();

private:
    nk_context ctx;
};
}  // namespace nk_impl

#endif /* NUKLEAR_IMPL_H_ */