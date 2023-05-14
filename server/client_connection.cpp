#include "client_connection.hpp"

#include "core/component/actor_command.hpp"
#include "core/utility/server_message.hpp"

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

void ClientConnection::handle_message(std::array<char, 4096> message_data) {
    m_last_message_time = std::chrono::steady_clock::now();

    std::string message{message_data.data()};
    spdlog::info("Received message from {}:{} : {}",
                 m_endpoint.address().to_string(),
                 m_endpoint.port(),
                 message);

    core::utility::ServerMessage server_message =
        core::utility::ServerMessage::deserialize(message);
    if (server_message.command().commands() != core::component::ActorCommand::Type::None) {
        m_entity.set<core::component::ActorCommand>(server_message.command());
    }
    m_entity.get_mut<std::vector<core::utility::ServerMessage::Acknowledgement>>()->emplace_back(
        server_message.sequence());
}

}  // namespace server
