#include "systems.hpp"

#include "spdlog/spdlog.h"

#include "core/utility/server_message.hpp"

namespace server::system {

void accumulate_actor_commands_for_propagation(flecs::world& ecs) {
    ecs.system<const core::component::ActorCommand>("AccumulateActorCommands")
        .kind(flecs::OnUpdate)
        .iter([&](flecs::iter& iter) {
            auto server_messages = ecs.get_mut<std::vector<core::utility::ServerMessage>>();
            for (size_t i = 0; i < iter.count(); ++i) {
                flecs::entity e = iter.entity(i);
                auto actor_command = e.get<core::component::ActorCommand>();
                if (actor_command->commands() == core::component::ActorCommand::Type::None) {
                    continue;
                }

                core::utility::ServerMessage server_message;
                server_message.set_actor(std::string{e.name()});
                server_message.set_command(*actor_command);

                server_messages->emplace_back(server_message);
            }
        });
}

}  // namespace server::system
