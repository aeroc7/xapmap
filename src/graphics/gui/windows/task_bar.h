/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TASK_BAR_H_
#define TASK_BAR_H_

#include <prog_state.h>

struct nk_context;

namespace graphics {
class TaskBar final {
public:
    void draw(const xapmap::CurState &prog, nk_context *ctx);

    static constexpr auto WINDOW_HEIGHT = 35;

private:
};
}  // namespace graphics

#endif /* TASK_BAR_H_ */