#include "systems.hpp"

#include "spdlog/spdlog.h"

#include "core/utility/server_broadcast.hpp"

#include "server/client_connection.hpp"

namespace server::system {

void send_server_broadcast(flecs::world& ecs) {
    ecs.system<ClientConnection>("SendServerBroadcast")
        .kind(flecs::OnUpdate)
        .iter([&](flecs::iter& iter) {
            spdlog::info("SendServerBroadcast");
            auto server_broadcast = ecs.get<core::utility::ServerBroadcast>();
            if (server_broadcast == nullptr) {
                spdlog::info("No server broadcast to send");
                return;
            }
            for (size_t i = 0; i < iter.count(); ++i) {
                flecs::entity e = iter.entity(i);
                auto client_connection = e.get_mut<ClientConnection>();

                spdlog::info("Sending server broadcast to: {}:{}, message: {}",
                             client_connection->endpoint().address().to_string(),
                             client_connection->endpoint().port(),
                             server_broadcast->serialize_str());
                client_connection->server_socket()->async_send_to(
                    asio::buffer(server_broadcast->serialize_str()),
                    client_connection->endpoint(),
                    [](std::error_code ec, std::size_t) {
                        if (ec) {
                            spdlog::error("Error sending message: {}", ec.message());
                        }
                    });
            }
        });
}

}  // namespace server::system
