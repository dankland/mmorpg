#include "systems.hpp"

#include <iostream>

#include "core/component/actor_command.hpp"

namespace core::system {

void load_actor_command(flecs::world& ecs, flecs::entity_t stage) {
    ecs.system<core::component::ActorCommandBuffer, core::component::ActorCommand>(
           "LoadActorCommand")
        .kind(stage)
        .each([](component::ActorCommandBuffer& actor_command_buffer,
                 component::ActorCommand& actor_command) {
            actor_command.clear();
            if (!actor_command_buffer.empty()) {
                actor_command = actor_command_buffer.pop();
            }
        });
}

}  // namespace core::system
