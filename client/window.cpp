#include "window.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include <stdexcept>

[[nodiscard]] std::string glewGetVersionString() {
#ifdef GLEW_VERSION_2_2
    return "2.2";
#elif defined(GLEW_VERSION_2_1)
    return "2.1";
#elif defined(GLEW_VERSION_2_0)
    return "2.0";
#else
    return "?";
#endif
}

void key_callback(GLFWwindow* window,
                  const int key,
                  const int scancode,
                  const int action,
                  const int mods) {
    assert(window);
    auto data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

    data->key_states[key] = action != GLFW_RELEASE;
}

void window_size_callback(GLFWwindow* window, const int width, const int height) {
    assert(window);
    auto data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

    data->width = width;
    data->height = height;

    glViewport(0, 0, width, height);
}

Window::Window(const int width, const int height, const std::string_view title) {
    // glfw
    const int success = glfwInit();
    if (!success) {
        throw std::runtime_error("glfw init failure");
    }

    // Create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    m_window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    if (m_window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(m_window, &m_data);

    // Callbacks
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetWindowSizeCallback(m_window, window_size_callback);

    // glew
    glewExperimental = GL_TRUE;
    const GLenum err = glewInit();
    if (GLEW_OK != err) {
        throw std::runtime_error("glew init failure");
    }

    std::cout << "glfw version " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "."
              << GLFW_VERSION_REVISION << std::endl;
    std::cout << "glew version " << glewGetVersionString() << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::set_viewport() {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
}

void Window::clear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

[[nodiscard]] bool Window::should_close() const noexcept {
    return glfwWindowShouldClose(m_window);
}

[[nodiscard]] GLFWwindow* Window::data() {
    return m_window;
}

void Window::swap() {
    glfwSwapBuffers(m_window);
}

void Window::poll() {
    glfwPollEvents();
}

[[nodiscard]] bool Window::get_key(const Inputs key) {
    switch (key) {
        case Inputs::KEY_ESCAPE:
            return m_data.key_states[GLFW_KEY_ESCAPE];
        case Inputs::KEY_W:
            return m_data.key_states[GLFW_KEY_W];
        case Inputs::KEY_S:
            return m_data.key_states[GLFW_KEY_S];
        case Inputs::KEY_A:
            return m_data.key_states[GLFW_KEY_A];
        case Inputs::KEY_D:
            return m_data.key_states[GLFW_KEY_D];
        default:
            return false;
    }
}
