/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef DEG_TO_RAD_H_
#define DEG_TO_RAD_H_

namespace utils {
template <typename T>
[[nodiscard]] T deg_to_rad(T deg) noexcept {
    return deg * (M_PI / 180.0);
}
}  // namespace utils

#endif /* DEG_TO_RAD_H_ */