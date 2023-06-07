#include "systems.hpp"

#include "core/component/actor_command.hpp"
#include "core/component/velocity.hpp"

namespace core::system {

void update_velocity(flecs::world& ecs, flecs::entity_t stage) {
    static float velocity_magnitude = 1.0f;
    ecs.system<const core::component::ActorCommand, core::component::Velocity>("UpdateVelocity")
        .kind(stage)
        .each([](const core::component::ActorCommand& character_command,
                 core::component::Velocity& velocity) {
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            if (character_command.contains(core::component::ActorCommand::MoveUp)) {
                velocity.y = velocity_magnitude;
            }
            if (character_command.contains(core::component::ActorCommand::MoveLeft)) {
                velocity.x = -velocity_magnitude;
            }
            if (character_command.contains(core::component::ActorCommand::MoveDown)) {
                velocity.y = -velocity_magnitude;
            }
            if (character_command.contains(core::component::ActorCommand::MoveRight)) {
                velocity.x += velocity_magnitude;
            }
        });
}

}  // namespace core::system
