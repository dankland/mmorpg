#ifndef MMORPG_CLIENT_RENDERER_HPP
#define MMORPG_CLIENT_RENDERER_HPP

#include <glm/glm.hpp>
#include "block.hpp"

class [[nodiscard]] OpenGLRenderer {
   public:
    OpenGLRenderer();

    ~OpenGLRenderer();

    void start();

    void end();

    /**
     * @brief Draw a quad bro
     *
     * @param pos Bottom left corner
     * @param size Width and height
     * @param rgba red, green, blue, alpha
     * @param layer Layer
     */
    void draw_quad(const glm::vec2 pos,
                   const glm::vec2 size,
                   const glm::vec4 rgba,
                   const float layer = 0.0f);

   private:
    BlockProgram m_block_program;
};

#endif
