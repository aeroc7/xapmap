/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PF_WRAPPER_H_
#define PF_WRAPPER_H_

#include <string>

namespace utils {
long gettime_highres();
long gettime_seconds();
std::string formatted_logtime();
}  // namespace utils

#endif /* PF_WRAPPER_H_ */