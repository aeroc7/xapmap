/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSERS_H_
#define PARSERS_H_

#include <atomic>
#include <thread>

namespace xapmap::parsers {
class Parsers {
public:
    Parsers();
    ~Parsers();

private:
    void thread_func();
    std::thread thread;
    std::atomic_bool quit_thread{false};
};
}  // namespace xapmap::parsers

#endif /* PARSERS_H_ */