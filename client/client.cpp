#include <iostream>
#include <stdexcept>
#include "core/component/position.hpp"
#include "renderer.hpp"
#include "window.hpp"

int main() {
    core::component::Position position = {-0.5f, -0.5f};

    try {
        auto window = Window(800, 600, "MMORPG");
        auto renderer = OpenGLRenderer();

        while (!window.should_close()) {
            window.poll();

            if (window.get_key(Inputs::KEY_ESCAPE)) {
                break;
            }

            if (window.get_key(Inputs::KEY_W)) {
                position.y += 0.01f;
            } else if (window.get_key(Inputs::KEY_S)) {
                position.y -= 0.01f;
            }
            if (window.get_key(Inputs::KEY_A)) {
                position.x -= 0.01f;
            } else if (window.get_key(Inputs::KEY_D)) {
                position.x += 0.01f;
            }

            window.clear();

            renderer.set_aspect(window.get_aspect());

            renderer.start();

            renderer.draw_quad(glm::vec2(0.0f, 0.0f),
                               glm::vec2(0.4f, 0.4f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                               -5.0f);

            renderer.draw_quad(glm::vec2(0.2f, 0.2f),
                               glm::vec2(0.4f, 0.4f),
                               glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                               5.0f);

            renderer.draw_quad(glm::vec2(position.x, position.y),
                               glm::vec2(0.2f, 0.2f),
                               glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
                               0.0f);

            renderer.end();

            window.swap();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
