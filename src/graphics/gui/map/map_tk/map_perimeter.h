/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MAP_PERIMETER_H_
#define MAP_PERIMETER_H_

#include "map_cmp.h"

namespace graphics {
class MapPerimeter : public MapCmp {
public:
private:
    void internal_draw(const xapmap::CurState &prog, const parsers::AirportData &data) override;
    void line_between(const xapmap::CurState &prog, const parsers::CoordPair &p1,
        const parsers::CoordPair &p2) const noexcept;
};
}  // namespace graphics

#endif /* MAP_PERIMETER_H_ */