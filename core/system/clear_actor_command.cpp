#include "systems.hpp"

#include "core/component/actor_command.hpp"

namespace core::system {

void clear_actor_command(flecs::world& ecs, flecs::entity_t stage) {
    ecs.system<core::component::ActorCommand>("ClearActorCommand")
        .kind(stage)
        .each([](core::component::ActorCommand& character_command) {
            character_command.clear();
        });
}

}  // namespace core::system
