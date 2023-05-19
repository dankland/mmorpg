#include "shader.hpp"
#include <stdexcept>
#include <string>

Shader::Shader(const Type type, const std::string& code) {
    switch (type) {
        case Type::Vertex:
            m_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case Type::Fragment:
            m_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            throw std::runtime_error("Unknown shader type");
    }

    if (!m_id) {
        throw std::runtime_error("Shader zero");
    }

    const char* source = code.c_str();
    // std::cout << "code:\n" << source << "\n";
    glShaderSource(m_id, 1, &source, nullptr);
    glCompileShader(m_id);

    // Check for compile time errors
    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(m_id, 512, nullptr, info_log);
        throw std::runtime_error("Vertex shader compilation failure\n" + std::string(info_log) +
                                 "\n");
    }
}

Shader::~Shader() {
    glDeleteShader(m_id);
}
