#include "systems.hpp"

#include "asio.hpp"
#include "spdlog/spdlog.h"

#include "core/utility/server_message.hpp"

#include "server/client_connection.hpp"

namespace server::system {

using udp = asio::ip::udp;

void acknowledge_server_messages(flecs::world& ecs) {
    ecs.system<ClientConnection, std::vector<core::utility::ServerMessage::Acknowledgement>>(
           "AcknowledgeServerMessages")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 ClientConnection& client_connection,
                 std::vector<core::utility::ServerMessage::Acknowledgement>& acknowledgements) {
            for (auto& ack : acknowledgements) {
                spdlog::info("Acknowledging seq {}", ack.sequence);
                core::utility::ServerMessage server_message;
                server_message.set_acknowledged_sequence(
                    core::utility::ServerMessage::Acknowledgement{ack.sequence});
                client_connection.server_socket()->async_send_to(
                    asio::buffer(server_message.serialize()),
                    client_connection.endpoint(),
                    [](std::error_code ec, std::size_t bytes_sent) {
                        if (ec) {
                            spdlog::error("Error sending message: {}", ec.message());
                        }
                    });
            }
            acknowledgements.clear();
        });
}

}  // namespace server::system
