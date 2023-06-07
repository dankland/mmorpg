#ifndef MMORPG_CORE_UTILITY_SERVER_BROADCAST_HPP
#define MMORPG_CORE_UTILITY_SERVER_BROADCAST_HPP

#include <atomic>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <msgpack.hpp>

#include "core/component/actor_command.hpp"
#include "core/component/position.hpp"

namespace core::utility {

struct ServerBroadcast {
    struct ActorState {
        std::string m_actor_id;
        core::component::ActorCommand m_actor_command;
        core::component::Position m_position;
        MSGPACK_DEFINE_MAP(m_actor_id, m_actor_command, m_position)
    };

    static ServerBroadcast create(std::uint64_t server_tick, std::vector<ActorState> actor_states) {
        return ServerBroadcast{
            .m_sequence = g_sequence++,
            .m_server_tick = server_tick,
            .m_actor_states = std::move(actor_states),
        };
    }

    [[nodiscard]] std::string serialize_str() const {
        std::string message;
        message += "seq " + std::to_string(m_sequence) + " ";
        message += "tick " + std::to_string(m_server_tick) + " ";
        for (const auto& actor_state : m_actor_states) {
            message += "actor " + actor_state.m_actor_id + " ";
            message += "command " + std::to_string(actor_state.m_actor_command.commands()) + " ";
            message += "position (" + std::to_string(actor_state.m_position.x) + "," +
                       std::to_string(actor_state.m_position.y) + ") ";
        }
        return message;
    }

    static ServerBroadcast deserialize_str(const std::string& data) {
        ServerBroadcast server_broadcast;
        std::istringstream iss(data);
        std::string token;
        while (iss >> token) {
            if (token == "seq") {
                iss >> server_broadcast.m_sequence;
            } else if (token == "tick") {
                iss >> server_broadcast.m_server_tick;
            } else if (token == "actor") {
                ActorState actor_state;
                iss >> actor_state.m_actor_id;
                iss >> token;
                if (token == "command") {
                    std::uint64_t command;
                    iss >> command;
                    actor_state.m_actor_command = core::component::ActorCommand{command};
                }
                iss >> token;
                if (token == "position") {
                    iss >> token;
                    std::string x = token.substr(1, token.size() - 2);
                    iss >> token;
                    std::string y = token.substr(0, token.size() - 1);
                    actor_state.m_position.x = std::stoi(x);
                    actor_state.m_position.y = std::stoi(y);
                }
                server_broadcast.m_actor_states.push_back(actor_state);
            }
        }
        return server_broadcast;
    }

    [[nodiscard]] std::string serialize() const {
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, *this);
        return {sbuf.data(), sbuf.size()};
    }

    static ServerBroadcast deserialize(const std::string& data) {
        msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
        msgpack::object obj = oh.get();
        return obj.as<ServerBroadcast>();
    }

    static std::atomic<std::uint64_t> g_sequence;

    std::uint64_t m_sequence{};
    std::uint64_t m_server_tick{};
    std::vector<ActorState> m_actor_states;
    MSGPACK_DEFINE_MAP(m_server_tick, m_sequence, m_actor_states)
};

inline std::atomic<std::uint64_t> ServerBroadcast::g_sequence = 1;

}  // namespace core::utility

#endif  // MMORPG_CORE_UTILITY_SERVER_BROADCAST_HPP
