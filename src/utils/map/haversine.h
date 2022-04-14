/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef HAVERSINE_H_
#define HAVERSINE_H_

#include <cmath>

#include "constants.h"
#include "deg_to_rad.h"

namespace utils {
// Kilometer-distance between two lat/lon points
template <typename T>
[[nodiscard]] constexpr T haversine_formula(T lat1, T lon1, T lat2, T lon2) noexcept {
    const auto rlat = deg_to_rad<T>(lat2 - lat1);
    const auto rlon = deg_to_rad<T>(lon2 - lon1);

    lat1 = deg_to_rad<T>(lat1);
    lat2 = deg_to_rad<T>(lat2);

    const T a = std::pow<T>(std::sin(rlat / 2.0), 2) +
                std::pow<T>(std::sin(rlon / 2.0), 2) * std::cos(lat1) * std::cos(lat2);
    const T c = 2.0 * std::asin(std::sqrt(a));
    return constants::EARTH_RADIUS * c;
}

template <typename T>
[[nodiscard]] constexpr T haversine_formula_meters(T lat1, T lon1, T lat2, T lon2) noexcept {
    return haversine_formula(lat1, lon1, lat2, lon2) * 1000.0;
}
}  // namespace utils

#endif /* HAVERSINE_H_ */