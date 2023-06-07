#include "client_connection.hpp"

#include "core/component/actor_command.hpp"
#include "core/utility/client_message.hpp"

#include "spdlog/spdlog.h"

namespace server {

ClientConnection::ClientConnection(udp::socket* server_socket,
                                   udp::endpoint endpoint,
                                   flecs::entity entity)
    : m_server_socket(server_socket),
      m_endpoint(std::move(endpoint)),
      m_entity(entity),
      m_last_message_time(std::chrono::steady_clock::now()) {
}

[[nodiscard]] udp::socket* ClientConnection::server_socket() {
    return m_server_socket;
}

const udp::endpoint& ClientConnection::endpoint() const {
    return m_endpoint;
}

bool ClientConnection::is_afk() const {
    return std::chrono::steady_clock::now() - m_last_message_time > std::chrono::seconds(30);
}

void ClientConnection::handle_message(const std::string& message) {
    m_last_message_time = std::chrono::steady_clock::now();

    spdlog::info("Received message from {}:{} : {}",
                 m_endpoint.address().to_string(),
                 m_endpoint.port(),
                 message);

    core::utility::ClientMessage client_message =
        core::utility::ClientMessage::deserialize_str(message);
    // spdlog::info("parsed message: {}", client_message.serialize_str());
    if (client_message.m_actor_command.commands() != core::component::ActorCommand::Type::None) {
        auto actor_command_buffer = m_entity.get_mut<core::component::ActorCommandBuffer>();
        actor_command_buffer->push(client_message.m_actor_command);
    }
}

}  // namespace server
