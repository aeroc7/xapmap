/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUI_MAIN_H_
#define GUI_MAIN_H_

#include <prog_state.h>

#include <memory>

#include "map/map_main.h"
#include "nuklear_impl/nuklear_wrapper.h"
#include "windows/airport_search.h"
#include "windows/parse_load.h"
#include "windows/task_bar.h"

namespace graphics {
class GuiMain {
public:
    void on_start(const xapmap::CurState &prog);
    void on_draw(const xapmap::CurState &prog);
    void on_stop(const xapmap::CurState &prog);

private:
    void gui_callback(const xapmap::CurState &prog, nk_context *ctx,
        xapmap::CurState::input_event_q_value_type &);
    std::unique_ptr<nk_impl::NkGui> gui;
    std::unique_ptr<graphics::MapMain> map_gui;

    AirportSearch ap_search_window;
    TaskBar task_bar_window;
    ParseLoad parse_load_window;
};
}  // namespace graphics

#endif /* GUI_MAIN_H_ */