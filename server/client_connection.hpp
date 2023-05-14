#ifndef MMORPG_SERVER_CLIENT_CONNECTION_HPP
#define MMORPG_SERVER_CLIENT_CONNECTION_HPP

#include "asio.hpp"
#include "flecs.h"

using udp = asio::ip::udp;

namespace server {

class ClientConnection {
   public:
    [[nodiscard]] explicit ClientConnection(udp::socket* server_socket,
                                            udp::endpoint endpoint,
                                            flecs::entity entity);

    [[nodiscard]] udp::socket* server_socket();
    [[nodiscard]] const udp::endpoint& endpoint() const;

    [[nodiscard]] bool is_afk() const;

    void handle_message(std::array<char, 4096> message);

    friend bool operator==(const ClientConnection& lhs, const ClientConnection& rhs) {
        return lhs.endpoint() == rhs.endpoint();
    }

   private:
    udp::socket* m_server_socket;
    udp::endpoint m_endpoint;
    flecs::entity m_entity;
    std::chrono::steady_clock::time_point m_last_message_time;
};

}  // namespace server

namespace std {
template <>
struct hash<server::ClientConnection> {
    std::size_t operator()(const server::ClientConnection& client_connection) const {
        return hash<std::string>()(client_connection.endpoint().address().to_string() + ":" +
                                   std::to_string(client_connection.endpoint().port()));
    }
};
}  // namespace std

#endif  // MMORPG_SERVER_CLIENT_CONNECTION_HPP
