/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSE_TASK_H_
#define PARSE_TASK_H_

#include <utils/utils.h>

#include <future>
#include <memory>
#include <optional>
#include <thread>

namespace parsers {
template <typename Task>
class ParseTask {
public:
    template <typename... Args>
    void start(Args... args) {
        std::packaged_task<std::unique_ptr<Task>(Args...)> pt(
            [](Args &&...larg) -> std::unique_ptr<Task> {
                return std::make_unique<Task>(std::forward<Args>(larg)...);
            });

        task_future = pt.get_future();
        task_thread = std::thread{std::move(pt), std::forward<Args>(args)...};
    }

    bool ready() noexcept {
        if (got_future_val) {
            return true;
        }

        const std::future_status task_finished = task_future.wait_for(std::chrono::milliseconds(0));
        ASSERT(task_finished != std::future_status::deferred);

        if (task_finished != std::future_status::ready) {
            return false;
        }

        return true;
    }

    std::optional<const Task *> get_data() {
        if (!task_handle && !got_future_val) {
            task_handle = task_future.get();
            got_future_val = true;
        }

        const auto task_ptr = task_handle.get();

        if (!task_ptr) {
            return std::nullopt;
        }

        return task_ptr;
    }

    ~ParseTask() {
        task_thread.join();
    }

private:
    std::thread task_thread;
    std::future<std::unique_ptr<Task>> task_future;
    std::unique_ptr<Task> task_handle;
    bool got_future_val{false};
};
}  // namespace parsers

#endif /* PARSE_TASK_H_ */