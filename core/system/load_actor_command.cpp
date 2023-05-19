#include "systems.hpp"

#include "core/component/actor_command.hpp"

namespace core::system {

void load_actor_command(flecs::world& ecs, flecs::entity_t stage) {
    ecs.system<core::component::ActorCommandBuffer>("LoadActorCommand")
        .kind(stage)
        .each([](flecs::entity e, component::ActorCommandBuffer& actor_command_buffer) {
            if (actor_command_buffer.empty()) {
                e.set<component::ActorCommand>(component::ActorCommand{});
            } else {
                e.set<component::ActorCommand>(actor_command_buffer.pop());
            }
        });
}

}  // namespace core::system
