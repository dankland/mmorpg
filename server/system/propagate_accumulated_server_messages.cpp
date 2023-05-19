#include "systems.hpp"

#include "spdlog/spdlog.h"

#include "core/utility/server_message.hpp"

#include "server/client_connection.hpp"

namespace server::system {

void propagate_accumulated_server_messages(flecs::world& ecs) {
    ecs.system<ClientConnection>("PropagateAccumulatedServerMessages")
        .kind(flecs::OnUpdate)
        .iter([&](flecs::iter& iter) {
            auto server_messages = ecs.get_mut<std::vector<core::utility::ServerMessage>>();
            for (size_t i = 0; i < iter.count(); ++i) {
                flecs::entity e = iter.entity(i);
                auto client_connection = e.get_mut<ClientConnection>();

                for (auto& server_message : *server_messages) {
                    client_connection->server_socket()->async_send_to(
                        asio::buffer(server_message.serialize()),
                        client_connection->endpoint(),
                        [](std::error_code ec, std::size_t) {
                            if (ec) {
                                spdlog::error("Error sending message: {}", ec.message());
                            }
                        });
                }
            }
            server_messages->clear();
        });
}

}  // namespace server::system
