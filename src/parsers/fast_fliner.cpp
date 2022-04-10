/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "fast_fliner.h"

#include <immintrin.h>
#include <utils/is_aligned.h>

#include <chrono>
#include <fstream>

#include "parse_except.h"

namespace parsers {
template <std::size_t Alignment, typename PtrType>
class AlignedLineBuf {
public:
    AlignedLineBuf(std::size_t size) {
        buf = new (std::align_val_t(Alignment)) PtrType[size];
    }

    AlignedLineBuf(AlignedLineBuf &&) noexcept = delete;
    AlignedLineBuf(const AlignedLineBuf &) = delete;
    AlignedLineBuf &operator=(AlignedLineBuf &&) noexcept = delete;
    AlignedLineBuf &operator=(const AlignedLineBuf &) = delete;

    PtrType *get_buf() const noexcept {
        return buf;
    }

    ~AlignedLineBuf() {
        if (buf) {
            ::operator delete[](buf, std::align_val_t(Alignment));
            buf = nullptr;
        }
    }

private:
    PtrType *buf{nullptr};
};

FastFliner::FastFliner(const std::string &filename, OnLineCallback cb, const std::atomic_bool &stop)
    : stop_parsing(stop) {
    if (cb == nullptr) {
        throw std::runtime_error("Invalid function callback");
    }

    user_func = cb;

    read_file(filename);
}

void FastFliner::read_file(const std::string &filename) const {
    auto file = std::ifstream{filename, std::ios::binary};

    if (!file) {
        throw ParseError("Failed to open file: " + filename);
    }

    AlignedLineBuf<SIMD_REG_SIZE, char> line_buf{FILE_READ_BLOCK_SIZE};

    int last_newline{0};
    std::size_t line_ctr{}, byte_ctr{};
    bool low_read{false};

    while (true) {
        const auto reread_amnt =
            ((last_newline > 0 && !low_read) ? (FILE_READ_BLOCK_SIZE - last_newline) : 0);
        if (reread_amnt > 0) {
            ASSERT(byte_ctr >= static_cast<std::size_t>(reread_amnt));
            file.seekg(
                static_cast<std::streamoff>(byte_ctr - static_cast<std::size_t>(reread_amnt)));
        }

        last_newline = 0;

        const auto succ_read_size =
            static_cast<int>(file.read(line_buf.get_buf(), FILE_READ_BLOCK_SIZE).gcount());
        low_read = (succ_read_size < FILE_READ_BLOCK_SIZE);

        if (succ_read_size > 0) {
            for (int i = 0; i < succ_read_size; i += SIMD_REG_SIZE) {
                const __m128i data_block =
                    _mm_load_si128(reinterpret_cast<const __m128i *>(line_buf.get_buf() + i));
                const __m128i delim_filled = _mm_set1_epi8(LINE_DELIMITER);
                const __m128i data_delim_eq = _mm_cmpeq_epi8(data_block, delim_filled);
                auto data_delim_eq_mask = _mm_movemask_epi8(data_delim_eq);

                const auto num_delims =
                    __builtin_popcount(static_cast<unsigned>(data_delim_eq_mask));

                for (int delims = 0; delims < num_delims; ++delims) {
                    ASSERT(data_delim_eq_mask != 0);
                    const auto line_end = __builtin_ffs(data_delim_eq_mask);
                    // Subtract 1 from line_end because __builtin_ffs returns (1 + index) of bit
                    const auto next_ind = i + (line_end - 1);
                    ASSERT(next_ind >= last_newline);

                    std::string_view cur_line = {
                        line_buf.get_buf() + last_newline, line_buf.get_buf() + next_ind};
                    user_func(LineData{line_ctr, cur_line});

                    last_newline = i + line_end;
                    // Clear next active bit
                    data_delim_eq_mask &= data_delim_eq_mask - 1;
                }

                line_ctr += static_cast<std::size_t>(num_delims);
            }

            byte_ctr += static_cast<std::size_t>(succ_read_size);
            byte_ctr -= static_cast<std::size_t>(reread_amnt);

            if (stop_parsing == false) [[likely]] {
                continue;
            }
        }

        break;
    }

    if (file.eof()) {
        return;
    } else if (file.bad()) {
        throw ParseError("I/O error while reading file " + filename);
    } else if (file.fail()) {
        throw ParseError("I/O stream read failed " + filename);
    }
}
}  // namespace parsers
