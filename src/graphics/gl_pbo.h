/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GL_PBO_H_
#define GL_PBO_H_

#include <GL/glew.h>

#include <atomic>
#include <functional>

namespace graphics {
class GlPbo {
public:
    GlPbo(GLsizei w, GLsizei h);
    void set_buffer_data(std::function<void(void *)> cb) noexcept;
    void bind_front_buffer() noexcept;
    ~GlPbo();

private:
    static constexpr std::size_t BUF_COUNT = 2;

    struct Buf {
        GLuint id;
        std::atomic<GLsizei> width, height;
    };

    void *cur_buf_handle{nullptr};
    std::array<Buf, BUF_COUNT> pbo_bufs{};

    std::atomic<GLuint> back_buf_index{1};
    std::atomic<GLuint> front_buf_index{0};
    std::atomic<bool> back_buf_ready{true};
    std::atomic<GLsizei> tgt_width, tgt_height;

    static_assert(BUF_COUNT == 2, "PBO can only have 2 buffers");
};
}  // namespace graphics

#endif /* GL_PBO_H_ */