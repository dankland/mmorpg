#include "server_connection.hpp"

#include "core/utility/client_message.hpp"
#include "core/utility/server_broadcast.hpp"
#include "spdlog/spdlog.h"

namespace client {

ServerConnection::ServerConnection(udp::socket* socket,
                                   udp::endpoint endpoint,
                                   flecs::entity entity)
    : m_socket(socket), m_endpoint(std::move(endpoint)), m_entity(entity) {
}

[[nodiscard]] udp::socket* ServerConnection::socket() {
    return m_socket;
}

const udp::endpoint& ServerConnection::endpoint() const {
    return m_endpoint;
}

void ServerConnection::handle_message(const std::string& message) {
    spdlog::info("Received message from {}:{} : {}",
                 m_endpoint.address().to_string(),
                 m_endpoint.port(),
                 message);

    core::utility::ServerBroadcast server_broadcast =
        core::utility::ServerBroadcast::deserialize_str(message);
    spdlog::info("parsed message: {}", server_broadcast.serialize_str());
}

}  // namespace client
