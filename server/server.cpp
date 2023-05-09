#include <iostream>

#include "core/component/position.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    mmorpg::Position position;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;

    return 0;
}
