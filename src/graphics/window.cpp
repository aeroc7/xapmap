/**
 * Copyright 2022 Bennett Anderson
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "window.h"

#include <utils/log.h>

using namespace utils;

namespace graphics {
void Window::window_glfw_error_callback([[maybe_unused]] int error, const char *description) {
    Log(Log::ERROR) << "glfw error: " << description;
}

Window::Window() {
    glfwSetErrorCallback(window_glfw_error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("glfwInit failed");
    }
}

void Window::window_loop(std::function<void()> func) {
    if (!func) {
        throw std::runtime_error("Invalid window loop cb passed");
    }

    while (!glfwWindowShouldClose(window)) {
        int win_width, win_height;
        glfwGetFramebufferSize(window, &win_width, &win_height);

        glViewport(0, 0, win_width, win_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, win_width, win_height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        // User-provided callback
        func();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::create_window(const std::string &title, int w, int h) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("Failed to create glfw window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

}  // namespace graphics