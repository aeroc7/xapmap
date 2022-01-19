/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "log.h"

#define VRET0(x)                                       \
    do {                                               \
        if ((x) != 0) [[unlikely]] {                   \
            utils::Log() << #x << " did not return 0"; \
            std::abort();                              \
        }                                              \
    } while (0)

#define ASSERT(x)                                            \
    do {                                                     \
        if (!(x)) [[unlikely]] {                             \
            utils::Log() << "Assertion " << #x << " failed"; \
            std::abort();                                    \
        }                                                    \
    } while (0)

#endif /* UTILS_H_ */