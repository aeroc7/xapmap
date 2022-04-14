/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ON_PARAM_CHANGE_H_
#define ON_PARAM_CHANGE_H_

#include <functional>

namespace utils {
template <typename T>
class OnParamChange {
public:
    void operator()(T new_val, std::function<void()> func) {
        if (last_val != new_val) {
            func();
            last_val = new_val;
        }
    }

private:
    T last_val;
};
}  // namespace utils

#endif /* ON_PARAM_CHANGE_H_ */