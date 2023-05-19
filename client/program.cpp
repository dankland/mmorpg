#include "program.hpp"
#include <stdexcept>
#include <string>

Program::Program(const std::vector<Shader>& shaders) {
    m_id = glCreateProgram();

    if (m_id == 0) {
        throw std::runtime_error("Program zero");
    }

    // Attach
    for (const auto& shader : shaders) {
        glAttachShader(m_id, shader.id());
    }

    glLinkProgram(m_id);

    // Check for linking errors
    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(m_id, 512, nullptr, info_log);
        throw std::runtime_error("Shader program link failure\n" + std::string(info_log) + "\n");
    }

    // Detach
    for (const auto& shader : shaders) {
        glDetachShader(m_id, shader.id());
    }
}

Program::~Program() {
}

void Program::clear() {
    glDeleteProgram(m_id);
}
