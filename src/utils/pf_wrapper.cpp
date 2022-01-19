/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "pf_wrapper.h"

#include <ctime>

#include "utils.h"

namespace utils {
long gettime_highres() {
#ifdef LIN
    struct timespec tp;
    VRET0(clock_gettime(CLOCK_MONOTONIC, &tp));
    return (tp.tv_sec * 1000000000L) + tp.tv_nsec;
#endif
}

long gettime_seconds() {
#ifdef LIN
    struct timespec tp;
    VRET0(clock_gettime(CLOCK_MONOTONIC, &tp));
    return tp.tv_sec;
#endif
}

std::string formatted_logtime() {
#ifdef LIN
    std::tm tbuf, *now;
    time_t t;
    char strbuf[16];

    t = std::time(nullptr);
    now = localtime_r(&t, &tbuf);
    std::strftime(strbuf, sizeof(strbuf), "%H:%M:%S", now);

    return strbuf;
#endif
}
}  // namespace utils