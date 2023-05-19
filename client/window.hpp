#ifndef MMORPG_CLIENT_WINDOW_HPP
#define MMORPG_CLIENT_WINDOW_HPP

#include <string_view>

struct GLFWwindow;

enum class Inputs
{
    KEY_ESCAPE = 0,
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
};

class [[nodiscard]] Window {
   public:
    struct Data {
        bool key_states[512] = {};
        int width = 800;
        int height = 600;
    };

    Window(const int width, const int height, const std::string_view title);

    ~Window();

    void set_viewport();

    [[nodiscard]] float get_aspect() const noexcept {
        if (m_data.width == 0) {
            return 1.0f;
        }
        return static_cast<float>(m_data.width) / m_data.height;
    }

    [[nodiscard]] bool should_close() const noexcept;

    [[nodiscard]] GLFWwindow* data();

    [[nodiscard]] bool get_key(const Inputs key);

    void clear();

    void swap();

    void poll();

   private:
    GLFWwindow* m_window = nullptr;
    Data m_data;
};

#endif
