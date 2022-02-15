/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GL_TEXTURE_H_
#define GL_TEXTURE_H_

#include <GL/glew.h>

namespace graphics {
class GlTexture final {
public:
    using size_type = int;
    using tex_type = GLuint;

    GlTexture(size_type width, size_type height);
    GlTexture(const GlTexture &) = delete;
    GlTexture &operator=(const GlTexture &) = delete;
    GlTexture(GlTexture &&) noexcept;
    GlTexture &operator=(GlTexture &&) noexcept;
    ~GlTexture();

    void bind() const noexcept;
    void regen_texture(size_type width, size_type height) noexcept;

private:
    static constexpr auto NULL_TEXTURE = 0;
    void try_delete_texture() noexcept;
    void create_new_texture(size_type width, size_type height) noexcept;

    tex_type tex_id{NULL_TEXTURE};
};
}  // namespace graphics

#endif /* GL_TEXTURE_H_ */