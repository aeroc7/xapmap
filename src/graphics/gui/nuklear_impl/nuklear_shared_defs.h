/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NUKLEAR_SHARED_DEFS_H_
#define NUKLEAR_SHARED_DEFS_H_

namespace nk_impl::nk_utils {
double strtod(const char *str, const char **endptr) noexcept;
char *dtoa(char *str, double n) noexcept;
}  // namespace nk_impl::nk_utils

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_BUTTON_TRIGGER_ON_RELEASE

#define NK_ASSERT(x) ASSERT(x)

#endif /* NUKLEAR_SHARED_DEFS_H_ */