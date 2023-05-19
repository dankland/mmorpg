#ifndef MMORPG_CLIENT_SHADER_HPP
#define MMORPG_CLIENT_SHADER_HPP

#include <string>

#include <GL/glew.h>

class [[nodiscard]] Shader {
   public:
    enum class Type
    {
        Vertex = 0,
        Fragment,
    };

    Shader(const Type type, const std::string& code);

    ~Shader();

    [[nodiscard]] GLuint id() const noexcept {
        return m_id;
    }

   private:
    GLuint m_id = 0;
};

#endif
