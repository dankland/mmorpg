#include <iostream>

#include "flecs.h"

#include "core/component/position.hpp"

void setup_entities(flecs::world& ecs) {
    ecs.entity("ExampleEntity").set<core::component::Position>({10, 20});
}

void setup_systems(flecs::world& ecs) {
    ecs.system<const core::component::Position>()
        .kind(flecs::OnUpdate)
        .each([](const core::component::Position& position) {
            std::cout << "Position: " << position.x << ", " << position.y << std::endl;
        });
}

int main() {
    flecs::world ecs;

    setup_entities(ecs);
    setup_systems(ecs);

    ecs.app().target_fps(60).run();

    return 0;
}
