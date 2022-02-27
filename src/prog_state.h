/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef STATE_H_
#define STATE_H_

#include <cairo/cairo.h>
#include <config/defaults.h>

#include <atomic>
#include <iostream>
#include <queue>

#include "graphics/window.h"

namespace xapmap {
class CurState {
public:
    cairo_t *cr{nullptr};
    std::atomic<int> window_width{dflt::DEFAULT_WINDOW_WIDTH};
    std::atomic<int> window_height{dflt::DEFAULT_WINDOW_HEIGHT};

    void add_cursor_event(graphics::CursorStats cs) const {
        std::lock_guard<std::mutex> lock{cursor_mut};
        if (cursor_event_q.size() == MAX_CURSOR_EVENTS) {
            std::cout << MAX_CURSOR_EVENTS << '\n';
            return;
        }
        cursor_event_q.push(cs);
    }

    // Boolean value will be true if the event is valid
    // Invalid event returned only if there are no more events
    std::tuple<graphics::CursorStats, bool> get_cursor_event() const {
        std::lock_guard<std::mutex> lock{cursor_mut};
        if (cursor_event_q.empty()) {
            return {{}, false};
        }

        graphics::CursorStats next_event = cursor_event_q.front();
        cursor_event_q.pop();
        return {next_event, true};
    }

private:
    static constexpr std::size_t MAX_CURSOR_EVENTS = 128;
    mutable std::mutex cursor_mut;
    mutable std::queue<graphics::CursorStats> cursor_event_q;
};
}  // namespace xapmap

#endif /* STATE_H_ */