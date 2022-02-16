/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "gl_texture.h"

#include <algorithm>

namespace graphics {

GlTexture::GlTexture(size_type width, size_type height) {
    regen_texture(width, height);
}

GlTexture::GlTexture(GlTexture &&o) noexcept : tex_id{std::exchange(o.tex_id, NULL_TEXTURE)} {
}

GlTexture &GlTexture::operator=(GlTexture &&o) noexcept {
    if (this != &o) {
        tex_id = std::exchange(o.tex_id, NULL_TEXTURE);
    }

    return *this;
}

void GlTexture::bind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, tex_id);
}

void GlTexture::unbind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GlTexture::regen_texture(int width, int height) noexcept {
    // Calls delete on texture
    create_new_texture(width, height);
}

void GlTexture::try_delete_texture() noexcept {
    if (tex_id > 0) {
        glDeleteTextures(1, &tex_id);
        tex_id = 0;
    }
}

void GlTexture::create_new_texture(int width, int height) noexcept {
    try_delete_texture();

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GlTexture::~GlTexture() {
    try_delete_texture();
}

}  // namespace graphics