#include "core/component/position.hpp"
#include "core/component/velocity.hpp"
#include "systems.hpp"

namespace core::system {

void update_position(flecs::world& ecs, flecs::entity_t stage) {
    ecs.system<const core::component::Velocity, core::component::Position>("UpdatePosition")
        .kind(stage)
        .each([](flecs::entity e,
                 const core::component::Velocity& velocity,
                 core::component::Position& position) {
            float dt = e.delta_time();
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
        });
}

}  // namespace core::system
