#include "block.hpp"
#include <stdexcept>
#define GLEW_STATIC
#include <GL/glew.h>
#include "program.hpp"
#include "renderer.hpp"
#include "shader.hpp"

static const GLchar* vertex_shader_source =
    "#version 330 core\n"
    "uniform mat4 view;\n"
    "layout (location = 0) in vec3 in_position;\n"
    "layout (location = 1) in vec4 in_colour;\n"
    "out vec4 out_colour;\n"
    "void main()\n"
    "{\n"
    "gl_Position = view * vec4(in_position, 1.0);\n"
    "out_colour = in_colour;\n"
    "}\n\0";

static const GLchar* fragment_shader_source =
    "#version 330 core\n"
    "in vec4 out_colour;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "colour = out_colour;\n"
    "}\n\0";

BlockProgram::BlockProgram() {
    const auto vertex_shader = Shader(Shader::Type::Vertex, vertex_shader_source);
    const auto fragment_shader = Shader(Shader::Type::Fragment, fragment_shader_source);
    m_program = Program({vertex_shader, fragment_shader});

    m_program.use();

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    if (m_vao == 0) {
        throw std::runtime_error("VAO zero");
    }

    if (m_vbo == 0) {
        throw std::runtime_error("VBO zero");
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BlockProgram::~BlockProgram() {
}

void BlockProgram::use() {
    m_program.use();
}

void BlockProgram::bind() {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void BlockProgram::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BlockProgram::clear() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}
