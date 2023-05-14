#ifndef MMORPG_CORE_COMPONENT_SERVER_MESSAGE_HPP
#define MMORPG_CORE_COMPONENT_SERVER_MESSAGE_HPP

#include <string>

#include "core/component/actor_command.hpp"

namespace core::utility {

struct ServerMessage {
    struct Acknowledgement {
        std::uint64_t sequence;
    };

    ServerMessage() : m_sequence{}, m_command{}, m_acknowledged_sequence{} {
    }

    [[nodiscard]] std::uint64_t sequence() const {
        return m_sequence;
    }

    void set_command(const core::component::ActorCommand& command) {
        m_command = command;
    }
    [[nodiscard]] const core::component::ActorCommand& command() const {
        return m_command;
    }

    void set_acknowledged_sequence(Acknowledgement sequence) {
        m_acknowledged_sequence = sequence;
    }
    [[nodiscard]] Acknowledgement acknowledged_sequence() const {
        return m_acknowledged_sequence;
    }

    [[nodiscard]] std::string serialize() const {
        std::string message;
        message += "seq " + std::to_string(g_sequence) + " ";
        if (m_command.commands() != core::component::ActorCommand::Type::None) {
            message += "command " + std::to_string(m_command.commands()) + " ";
        }
        if (m_acknowledged_sequence.sequence != 0) {
            message += "ack " + std::to_string(m_acknowledged_sequence.sequence) + " ";
        }
        message += "\n";
        return message;
    }
    static ServerMessage deserialize(const std::string& message) {
        static std::string sequence_prefix = "seq ";
        static std::string command_prefix = "command ";
        static std::string ack_prefix = "ack ";

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
            } else if (message.substr(pos, command_prefix.size()) == command_prefix) {
                pos += command_prefix.size();
                std::string::size_type next_space = message.find(' ', pos);
                if (next_space >= message.size()) {
                    next_space = message.size();
                }
                std::uint64_t command = std::stoull(message.substr(pos, next_space - pos));

                server_message.set_command(core::component::ActorCommand{command});

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
    core::component::ActorCommand m_command;
    Acknowledgement m_acknowledged_sequence;
};

inline std::uint64_t ServerMessage::g_sequence = 1;

}  // namespace core::utility

#endif  // MMORPG_CORE_COMPONENT_SERVER_MESSAGE_HPP
