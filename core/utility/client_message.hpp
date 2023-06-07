#ifndef MMORPG_CORE_UTILITY_CLIENT_MESSAGE_HPP
#define MMORPG_CORE_UTILITY_CLIENT_MESSAGE_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <msgpack.hpp>

#include "core/component/actor_command.hpp"
#include "core/component/position.hpp"

namespace core::utility {

struct ClientMessage {
    static ClientMessage create(core::component::ActorCommand m_actor_command) {
        return ClientMessage{
            .m_sequence = g_sequence++,
            .m_actor_command = m_actor_command,
        };
    }

    [[nodiscard]] std::string serialize_str() const {
        std::string message;
        message += "seq " + std::to_string(m_sequence) + " ";
        message += "cmd " + std::to_string(m_actor_command.commands());
        return message;
    }

    static ClientMessage deserialize_str(const std::string& data) {
        ClientMessage client_message;
        std::istringstream iss(data);
        std::string token;
        while (iss >> token) {
            if (token == "seq") {
                iss >> client_message.m_sequence;
            } else if (token == "cmd") {
                std::uint64_t command;
                iss >> command;
                client_message.m_actor_command = core::component::ActorCommand{command};
            }
        }
        return client_message;
    }

    [[nodiscard]] std::string serialize() const {
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, *this);
        return {sbuf.data(), sbuf.size()};
    }

    static ClientMessage deserialize(const std::string& data) {
        msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
        msgpack::object obj = oh.get();
        return obj.as<ClientMessage>();
    }

    MSGPACK_DEFINE_MAP(m_sequence, m_actor_command)

    static std::uint64_t g_sequence;

    std::uint64_t m_sequence{};
    core::component::ActorCommand m_actor_command;
};

inline std::uint64_t ClientMessage::g_sequence = 1;

}  // namespace core::utility

#endif  // MMORPG_CORE_UTILITY_CLIENT_MESSAGE_HPP
