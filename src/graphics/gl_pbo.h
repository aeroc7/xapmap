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
    void get_buffer_data(std::function<void(void *)> cb) noexcept;
    void bind_front_buffer() noexcept;
    ~GlPbo();

private:
    static constexpr std::size_t BUF_COUNT = 2;

    GLsizei width, height;
    void *cur_buf_handle{nullptr};
    GLuint pbo_bufs[BUF_COUNT];

    std::atomic<GLuint> back_buf_index{0};
    std::atomic<GLuint> front_buf_index{0};
    std::atomic<bool> back_buf_ready{true};
};
}  // namespace graphics

#endif /* GL_PBO_H_ */