#ifndef MMORPG_CORE_COMPONENT_SERVER_MESSAGE_HPP
#define MMORPG_CORE_COMPONENT_SERVER_MESSAGE_HPP

#include <string>

#include "core/component/actor_command.hpp"
#include "core/component/position.hpp"

namespace core::utility {

struct ServerMessage {
    struct Acknowledgement {
        std::uint64_t sequence;
    };

    ServerMessage()
        : m_sequence{}, m_acknowledged_sequence{}, m_actor{}, m_command{}, m_position{} {
    }

    [[nodiscard]] std::uint64_t sequence() const {
        return m_sequence;
    }

    void set_acknowledged_sequence(Acknowledgement sequence) {
        m_acknowledged_sequence = sequence;
    }
    [[nodiscard]] Acknowledgement acknowledged_sequence() const {
        return m_acknowledged_sequence;
    }

    void set_actor(const std::string& actor) {
        m_actor = actor;
    }
    [[nodiscard]] const std::string& actor() const {
        return m_actor;
    }

    void set_command(const core::component::ActorCommand& command) {
        m_command = command;
    }
    [[nodiscard]] const core::component::ActorCommand& command() const {
        return m_command;
    }

    void set_position(const core::component::Position& position) {
        m_position = position;
    }
    [[nodiscard]] const core::component::Position& position() const {
        return m_position;
    }

    [[nodiscard]] std::string serialize() const {
        std::string message;
        message += "seq " + std::to_string(g_sequence++) + " ";
        if (m_acknowledged_sequence.sequence != 0) {
            message += "ack " + std::to_string(m_acknowledged_sequence.sequence) + " ";
        }
        if (!m_actor.empty()) {
            message += "actor " + m_actor + " ";
        }
        if (m_command.commands() != core::component::ActorCommand::Type::None) {
            message += "command " + std::to_string(m_command.commands()) + " ";
        }
        if (m_position.x != -std::numeric_limits<float>::infinity() ||
            m_position.y != -std::numeric_limits<float>::infinity()) {
            message +=
                "pos (" + std::to_string(m_position.x) + "," + std::to_string(m_position.y) + ") ";
        }
        message += "\n";
        return message;
    }
    static ServerMessage deserialize(const std::string& message) {
        static std::string sequence_prefix = "seq ";
        static std::string ack_prefix = "ack ";
        static std::string actor_prefix = "actor ";
        static std::string command_prefix = "command ";
        static std::string position_prefix = "pos ";

        ServerMessage server_message;
        std::string::size_type pos = 0;
        while (pos < message.size()) {
            if (message.substr(pos, sequence_prefix.size()) == sequence_prefix) {
                pos += sequence_prefix.size();
                std::string::size_type next_space = message.find(' ', pos);
                if (next_space >= message.size()) {
                    next_space = message.size();
                }
                std::uint64_t sequence = std::stoull(message.substr(pos, next_space - pos));

                server_message.set_sequence(sequence);

                pos = next_space + 1;
            } else if (message.substr(pos, ack_prefix.size()) == ack_prefix) {
                pos += ack_prefix.size();
                std::string::size_type next_space = message.find(' ', pos);
                if (next_space >= message.size()) {
                    next_space = message.size();
                }
                std::uint64_t ack = std::stoull(message.substr(pos, next_space - pos));

                server_message.set_acknowledged_sequence(Acknowledgement{ack});

                pos = next_space + 1;
            } else if (message.substr(pos, actor_prefix.size()) == actor_prefix) {
                pos += actor_prefix.size();
                std::string::size_type next_space = message.find(' ', pos);
                if (next_space >= message.size()) {
                    next_space = message.size();
                }
                std::string actor = message.substr(pos, next_space - pos);

                server_message.set_actor(actor);

                pos = next_space + 1;
            } else if (message.substr(pos, command_prefix.size()) == command_prefix) {
                pos += command_prefix.size();
                std::string::size_type next_space = message.find(' ', pos);
                if (next_space >= message.size()) {
                    next_space = message.size();
                }
                std::uint64_t command = std::stoull(message.substr(pos, next_space - pos));

                server_message.set_command(core::component::ActorCommand{command});

                pos = next_space + 1;
            } else if (message.substr(pos, position_prefix.size()) == position_prefix) {
                pos += position_prefix.size();
                std::string::size_type next_space = message.find(' ', pos);
                if (next_space >= message.size()) {
                    next_space = message.size();
                }
                std::string position = message.substr(pos, next_space - pos);

                std::string::size_type comma = position.find(',');
                std::string x = position.substr(1, comma - 1);
                std::string y = position.substr(comma + 1, position.size() - comma - 2);

                server_message.set_position(core::component::Position{
                    std::stof(x),
                    std::stof(y),
                });

                pos = next_space + 1;
            } else {
                pos++;
            }
        }

        return server_message;
    }

   private:
    void set_sequence(std::uint64_t sequence) {
        m_sequence = sequence;
    }

    static std::uint64_t g_sequence;
    std::uint64_t m_sequence;
    Acknowledgement m_acknowledged_sequence;
    std::string m_actor;
    core::component::ActorCommand m_command;
    core::component::Position m_position;
};

inline std::uint64_t ServerMessage::g_sequence = 1;

}  // namespace core::utility

#endif  // MMORPG_CORE_COMPONENT_SERVER_MESSAGE_HPP
