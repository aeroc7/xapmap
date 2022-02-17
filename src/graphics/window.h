/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace graphics {
class Window {
public:
    Window();
    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;
    void create_window(const std::string &title, int w, int h);
    void window_loop(std::function<void()> func);
    std::tuple<unsigned, unsigned> get_window_dims() const {
        return {window_width, window_height};
    }
    ~Window();

private:
    static void glfw_error_callback(int error, const char *description);
    GLFWwindow *window{nullptr};
    int window_width{}, window_height{};
};
}  // namespace graphics

#endif /* WINDOW_H_ */