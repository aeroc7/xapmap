/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gl_pbo.h"

#include <utils/utils.h>

namespace graphics {
GlPbo::GlPbo(GLsizei w, GLsizei h) : tgt_width(w), tgt_height(h) {
    for (auto &buf : pbo_bufs) {
        buf.width = w;
        buf.height = h;

        glGenBuffers(1, &buf.id);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buf.id);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, buf.width * buf.height * 4, nullptr, GL_DYNAMIC_DRAW);
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[0].id);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, tgt_width, tgt_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    cur_buf_handle = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void GlPbo::set_buffer_data(std::function<void(void *)> cb) noexcept {
    if (!back_buf_ready || !cb) {
        return;
    }

    ASSERT(cur_buf_handle != nullptr);
    cb(cur_buf_handle);
    back_buf_ready = false;
}

void GlPbo::bind_front_buffer() noexcept {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[front_buf_index].id);

    if (!back_buf_ready) {
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0, tgt_width, tgt_height, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[back_buf_index].id);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

        back_buf_index = (back_buf_index + 1) % 2;
        front_buf_index = (front_buf_index + 1) % 2;

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[back_buf_index].id);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, tgt_width * tgt_height * 4, nullptr, GL_DYNAMIC_DRAW);
        cur_buf_handle = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

        back_buf_ready = true;
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GlPbo::~GlPbo() {
    for (const auto &buf : pbo_bufs) {
        glDeleteBuffers(1, &buf.id);
    }
}
}  // namespace graphics