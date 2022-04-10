/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef FAST_FLINER_H_
#define FAST_FLINER_H_

#include <atomic>
#include <functional>
#include <string>

namespace parsers {
struct LineData {
    std::size_t line_number;
    std::string_view line_data;
};
// Fast-File-Line-Reader
class FastFliner {
public:
    using OnLineCallback = std::function<void(const LineData &)>;  // void (*)(const LineData &ld);
    FastFliner(const std::string &filename, OnLineCallback cb, const std::atomic_bool &stop);

private:
    void read_file(const std::string &filename) const;
    OnLineCallback user_func;
    const std::atomic_bool &stop_parsing;

    static constexpr std::size_t SIMD_REG_SIZE = 16;
    static constexpr auto FILE_READ_BLOCK_SIZE = 1024 * 8;
    static constexpr auto LINE_DELIMITER = '\n';
};
}  // namespace parsers

#endif /* FAST_FLINER_H_ */