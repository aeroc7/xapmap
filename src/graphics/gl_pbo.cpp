/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gl_pbo.h"

namespace graphics {
GlPbo::GlPbo(GLsizei w, GLsizei h) : width(w), height(h) {
    glGenBuffers(2, pbo_bufs);

    for (std::size_t i = 0; i < BUF_COUNT; ++i) {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[i]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, nullptr, GL_DYNAMIC_DRAW);
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[0]);
    cur_buf_handle = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void GlPbo::set_buffer_data(std::function<void(void *)> cb) noexcept {
    if (!back_buf_ready || !cb) {
        return;
    }

    cb(cur_buf_handle);
    back_buf_ready = false;
}

void GlPbo::bind_front_buffer() noexcept {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[front_buf_index]);

    if (!back_buf_ready) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[back_buf_index]);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

        back_buf_index = (back_buf_index + 1) % 2;
        front_buf_index = (front_buf_index + 1) % 2;

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[back_buf_index]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, nullptr, GL_DYNAMIC_DRAW);
        cur_buf_handle = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

        back_buf_ready = true;
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GlPbo::~GlPbo() {
    glDeleteBuffers(2, pbo_bufs);
}
}  // namespace graphics