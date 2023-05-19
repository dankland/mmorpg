#include "systems.hpp"

#include "spdlog/spdlog.h"

#include "server/client_connection.hpp"

namespace server::system {

void afk_check(flecs::world& ecs) {
    ecs.system<const ClientConnection>("AFKCheck")
        .kind(flecs::OnUpdate)
        .interval(300.0f)
        .iter([&](flecs::iter& iter) {
            spdlog::info("Running afk check system");
            auto client_connections =
                ecs.get_mut<std::unordered_map<udp::endpoint, flecs::entity>>();
            for (size_t i = 0; i < iter.count(); ++i) {
                flecs::entity e = iter.entity(i);
                auto client_connection = e.get_mut<ClientConnection>();
                if (!client_connections->contains(client_connection->endpoint())) {
                    continue;
                }
                if (client_connection->is_afk()) {
                    spdlog::info("Kicking client: {}:{}",
                                 e.get<ClientConnection>()->endpoint().address().to_string(),
                                 e.get<ClientConnection>()->endpoint().port());
                    e.destruct();
                    client_connections->erase(client_connection->endpoint());
                }
            }
        });
}

}  // namespace server::system
