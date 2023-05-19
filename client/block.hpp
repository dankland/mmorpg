#ifndef MMORPG_CLIENT_BLOCK_HPP
#define MMORPG_CLIENT_BLOCK_HPP

#include <GL/glew.h>

#include "program.hpp"

class [[nodiscard]] BlockProgram {
   public:
    BlockProgram();

    ~BlockProgram();

    void use();

    void bind();

    void unbind();

    void clear();

   private:
    Program m_program;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};

#endif
