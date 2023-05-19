#include "renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<GLfloat> vertices;

OpenGLRenderer::OpenGLRenderer() {
}

OpenGLRenderer::~OpenGLRenderer() {
    m_block_program.clear();
}

void OpenGLRenderer::start() {
}

void OpenGLRenderer::end() {
    m_block_program.use();
    m_block_program.bind();

    // Camera matrix
    const auto near = 0.0f;
    const auto far = 100.0f;
    const auto position = glm::vec3(0.0, 0.0, 50.0);
    const auto projection = glm::ortho(-1.0f * m_aspect, 1.0f * m_aspect, -1.0f, 1.0f, near, far);
    const auto view =
        glm::lookAt(position, position - glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const auto matrix = projection * view;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(matrix));

    glBufferData(
        GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 7);

    m_block_program.unbind();

    vertices.clear();
}

void OpenGLRenderer::draw_quad(const glm::vec2 pos,
                               const glm::vec2 size,
                               const glm::vec4 rgba,
                               const float layer) {
    // v1
    vertices.emplace_back(pos.x);
    vertices.emplace_back(pos.y);
    vertices.emplace_back(layer);
    vertices.emplace_back(rgba.r);
    vertices.emplace_back(rgba.g);
    vertices.emplace_back(rgba.b);
    vertices.emplace_back(rgba.a);

    // v2
    vertices.emplace_back(pos.x);
    vertices.emplace_back(pos.y + size.y);
    vertices.emplace_back(layer);
    vertices.emplace_back(rgba.r);
    vertices.emplace_back(rgba.g);
    vertices.emplace_back(rgba.b);
    vertices.emplace_back(rgba.a);

    // v3
    vertices.emplace_back(pos.x + size.x);
    vertices.emplace_back(pos.y + size.y);
    vertices.emplace_back(layer);
    vertices.emplace_back(rgba.r);
    vertices.emplace_back(rgba.g);
    vertices.emplace_back(rgba.b);
    vertices.emplace_back(rgba.a);

    // v4
    vertices.emplace_back(pos.x);
    vertices.emplace_back(pos.y);
    vertices.emplace_back(layer);
    vertices.emplace_back(rgba.r);
    vertices.emplace_back(rgba.g);
    vertices.emplace_back(rgba.b);
    vertices.emplace_back(rgba.a);

    // v5
    vertices.emplace_back(pos.x + size.x);
    vertices.emplace_back(pos.y + size.y);
    vertices.emplace_back(layer);
    vertices.emplace_back(rgba.r);
    vertices.emplace_back(rgba.g);
    vertices.emplace_back(rgba.b);
    vertices.emplace_back(rgba.a);

    // v6
    vertices.emplace_back(pos.x + size.x);
    vertices.emplace_back(pos.y);
    vertices.emplace_back(layer);
    vertices.emplace_back(rgba.r);
    vertices.emplace_back(rgba.g);
    vertices.emplace_back(rgba.b);
    vertices.emplace_back(rgba.a);
}
