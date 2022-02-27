/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <functional>
#include <string>

struct GLFWwindow;

namespace graphics {
enum class CursorStatType { MOUSE_MOVE, LEFT_MOUSE_PRESS, LEFT_MOUSE_RELEASE };
struct CursorStats {
    CursorStatType type;
    std::uint_fast32_t x_pos;
    std::uint_fast32_t y_pos;
};

class Window {
public:
    using CursorCbSignature = std::function<void(CursorStats)>;
    Window();
    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;
    void create_window(const std::string &title, int w, int h);
    void window_loop(std::function<void()> func);
    std::tuple<int, int> get_window_dims() const {
        return {window_width, window_height};
    }

    void set_cursor_cb(CursorCbSignature cb) {
        cursor_cb = cb;
    }

    ~Window();

private:
    static void glfw_cursor_button_callback(
        GLFWwindow *window, int button, int action, int mods) noexcept;
    static void glfw_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
    static void glfw_error_callback(int error, const char *description);
    GLFWwindow *window{nullptr};
    int window_width{}, window_height{};
    CursorCbSignature cursor_cb;
};
}  // namespace graphics

#endif /* WINDOW_H_ */