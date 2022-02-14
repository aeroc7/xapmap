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
    ~Window();

private:
    static void window_glfw_error_callback([[maybe_unused]] int error, const char *description);
    GLFWwindow *window{nullptr};
};
}  // namespace graphics

#endif /* WINDOW_H_ */