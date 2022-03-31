/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef IS_ALIGNED_H_
#define IS_ALIGNED_H_

#include <cstdint>

namespace utils {
template <long Boundary, typename T>
constexpr bool is_aligned_to(const T *ptr) noexcept {
    return reinterpret_cast<std::uintptr_t>(static_cast<const void *>(ptr)) % Boundary == 0;
}
}  // namespace utils

#endif /* IS_ALIGNED_H_ */