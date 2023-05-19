#ifndef MMORPG_CLIENT_PROGRAM_HPP
#define MMORPG_CLIENT_PROGRAM_HPP

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include "shader.hpp"

class [[nodiscard]] Program {
   public:
    Program() = default;

    Program(const std::vector<Shader>& shaders);

    ~Program();

    [[nodiscard]] GLuint id() const noexcept {
        return m_id;
    }

    void use() const noexcept {
        glUseProgram(m_id);
    }

    void clear();

   private:
    GLuint m_id = 0;
};

#endif
