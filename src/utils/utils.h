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

#define VERIF0(x)                                                            \
    do {                                                                     \
        if ((x) != 0) [[unlikely]] {                                         \
            utils::Log(utils::Log::ERROR) << #x << " did not evaluate to 0"; \
            std::abort();                                                    \
        }                                                                    \
    } while (0)

#define ASSERT(x)                                                             \
    do {                                                                      \
        if (!(x)) [[unlikely]] {                                              \
            utils::Log(utils::Log::ERROR) << "Assertion " << #x << " failed"; \
            std::abort();                                                     \
        }                                                                     \
    } while (0)

#endif /* UTILS_H_ */