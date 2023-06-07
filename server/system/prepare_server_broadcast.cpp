#include "systems.hpp"

#include "spdlog/spdlog.h"

#include "core/utility/server_broadcast.hpp"

namespace server::system {

void prepare_server_broadcast(flecs::world& ecs) {
    ecs.system<const core::component::ActorCommand, const core::component::Position>(
           "PrepareServerBroadcast")
        .kind(flecs::OnUpdate)
        .iter([&](flecs::iter& iter) {
            spdlog::info("PrepareServerBroadcast");
            ecs.set<core::utility::ServerBroadcast>(
                core::utility::ServerBroadcast::create(ecs.tick(), {}));
            auto server_broadcast = ecs.get_mut<core::utility::ServerBroadcast>();
            for (size_t i = 0; i < iter.count(); ++i) {
                flecs::entity e = iter.entity(i);
                auto actor_command = e.get<core::component::ActorCommand>();
                auto position = e.get<core::component::Position>();

                server_broadcast->m_actor_states.emplace_back(
                    core::utility::ServerBroadcast::ActorState{
                        .m_actor_id = {e.name()},
                        .m_actor_command = *actor_command,
                        .m_position = *position,
                    });
            }
        });
}

}  // namespace server::system
