#ifndef MMORPG_CLIENT_SERVER_CONNECTION_HPP
#define MMORPG_CLIENT_SERVER_CONNECTION_HPP

#include "asio.hpp"
#include "flecs.h"

using udp = asio::ip::udp;

namespace client {

class ServerConnection {
   public:
    [[nodiscard]] explicit ServerConnection(udp::socket* socket,
                                            udp::endpoint endpoint,
                                            flecs::entity entity);

    [[nodiscard]] udp::socket* socket();
    [[nodiscard]] const udp::endpoint& endpoint() const;

    void handle_message(const std::string& message);

    friend bool operator==(const ServerConnection& lhs, const ServerConnection& rhs) {
        return lhs.endpoint() == rhs.endpoint();
    }

   private:
    udp::socket* m_socket;
    udp::endpoint m_endpoint;
    flecs::entity m_entity;
};

}  // namespace client

namespace std {
template <>
struct hash<client::ServerConnection> {
    std::size_t operator()(const client::ServerConnection& server_connection) const {
        return hash<std::string>()(server_connection.endpoint().address().to_string() + ":" +
                                   std::to_string(server_connection.endpoint().port()));
    }
};
}  // namespace std

#endif  // MMORPG_CLIENT_SERVER_CONNECTION_HPP
