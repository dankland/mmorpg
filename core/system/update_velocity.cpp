#include "systems.hpp"

#include "core/component/actor_command.hpp"
#include "core/component/velocity.hpp"

namespace core::system {

void update_velocity(flecs::world& ecs, flecs::entity_t stage) {
    ecs.system<const core::component::ActorCommand, core::component::Velocity>("UpdateVelocity")
        .kind(stage)
        .each([](const core::component::ActorCommand& character_command,
                 core::component::Velocity& velocity) {
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            if (character_command.contains(core::component::ActorCommand::MoveUp)) {
                velocity.y -= 100.0f;
            }
            if (character_command.contains(core::component::ActorCommand::MoveLeft)) {
                velocity.x -= 100.0f;
            }
            if (character_command.contains(core::component::ActorCommand::MoveDown)) {
                velocity.y += 100.0f;
            }
            if (character_command.contains(core::component::ActorCommand::MoveRight)) {
                velocity.x += 100.0f;
            }
        });
}

}  // namespace core::system
