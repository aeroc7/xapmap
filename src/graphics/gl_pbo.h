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

namespace graphics {
class GlPbo {
public:
    using PboBuf = void *;
    GlPbo(GLsizei w, GLsizei h);
    PboBuf get_back_buffer() const noexcept;
    void finish_back_buffer() noexcept;
    void bind_front_buffer() noexcept;
    ~GlPbo();

private:
    GLsizei width;
    GLsizei height;
    PboBuf cur_buf_handle;
    GLuint pbo_bufs[2];

    std::atomic<GLuint> back_buf_ind{};
    std::atomic<GLuint> front_buf_ind{};
    std::atomic<bool> back_buf_ready{};
    std::atomic<bool> back_buf_done{};
};
}  // namespace graphics

#endif /* GL_PBO_H_ */