/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MAP_CMP_H_
#define MAP_CMP_H_

#include <parsers/apt_dat.h>
#include <prog_state.h>

namespace graphics {
class MapCmp {
public:
    void draw(const xapmap::CurState &prog, const parsers::AirportData &data);
    void show() noexcept;
    void hide() noexcept;
    bool is_visible() const noexcept;

    virtual ~MapCmp() = default;

protected:
    virtual void internal_draw(const xapmap::CurState &prog, const parsers::AirportData &data) = 0;
    bool should_draw{true};

private:
};
}  // namespace graphics

#endif /* MAP_CMP_H_ */