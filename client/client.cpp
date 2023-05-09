#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLchar* vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 in_position;\n"
    "layout (location = 1) in vec4 in_colour;\n"
    "out vec4 out_colour;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(in_position, 1.0);\n"
    "out_colour = in_colour;\n"
    "}\0";

const GLchar* fragment_shader_source =
    "#version 330 core\n"
    "in vec4 out_colour;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "colour = out_colour;\n"
    "}\n\0";

void key_callback(GLFWwindow* window, const int key, const int, const int action, const int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    // glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "MMORPG", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Callbacks
    glfwSetKeyCallback(window, key_callback);

    // glew
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLchar info_log[512];

    // Vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    // Check for compile time errors
    GLint success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cerr << "Vertex shader compilation failure\n" << info_log << std::endl;
        return 1;
    }

    // Fragment shader
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    // Check for compile time errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cerr << "Fragment shader compilation failure\n" << info_log << std::endl;
        return 1;
    }

    // Link shaders
    const GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // Check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        std::cerr << "Shader program link failure\n" << info_log << std::endl;
        return 1;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // clang-format off
    const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };
    // clang-format on

    GLuint vbo = 0;
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();

    return 0;
}
