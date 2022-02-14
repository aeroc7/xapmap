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

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, nullptr, GL_DYNAMIC_DRAW);

    cur_buf_handle = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    back_buf_ind = 0;
    front_buf_ind = 1;
    back_buf_ready = true;
    back_buf_done = false;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, nullptr, GL_DYNAMIC_DRAW);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GlPbo::PboBuf GlPbo::get_back_buffer() const noexcept {
    if (back_buf_ready) {
        return cur_buf_handle;
    }

    return nullptr;
}

void GlPbo::finish_back_buffer() noexcept {
    back_buf_done = true;
    back_buf_ready = false;
}

void GlPbo::bind_front_buffer() noexcept {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[front_buf_ind]);

    if (back_buf_done) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[back_buf_ind]);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

        back_buf_ind = (back_buf_ind + 1) % 2;
        front_buf_ind = (front_buf_ind + 1) % 2;

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_bufs[back_buf_ind]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, nullptr, GL_DYNAMIC_DRAW);
        cur_buf_handle = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

        back_buf_done = false;
        back_buf_ready = true;
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GlPbo::~GlPbo() {
    glDeleteBuffers(2, pbo_bufs);
    cur_buf_handle = nullptr;
}
}  // namespace graphics