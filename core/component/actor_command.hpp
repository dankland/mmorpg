#ifndef MMORPG_CORE_COMPONENT_ACTOR_COMMAND_HPP
#define MMORPG_CORE_COMPONENT_ACTOR_COMMAND_HPP

#include <cstdint>
#include <unordered_set>

namespace core::component {

struct ActorCommand {
    enum Type : std::uint64_t
    {
        None = 0,

        MoveUp = std::uint64_t(1) << 0,
        MoveLeft = std::uint64_t(1) << 1,
        MoveDown = std::uint64_t(1) << 2,
        MoveRight = std::uint64_t(1) << 3,
    };

    ActorCommand() : m_commands{} {
    }
    explicit ActorCommand(std::uint64_t commands) : m_commands{commands} {
    }

    [[nodiscard]] bool contains(Type command) const {
        return m_commands & command;
    }
    void add(Type command) {
        m_commands |= command;
    }
    void remove(Type command) {
        m_commands &= ~command;
    }
    void clear() {
        m_commands = 0;
    }
    [[nodiscard]] std::uint64_t commands() const {
        return m_commands;
    }

   private:
    std::uint64_t m_commands;
};

}  // namespace core::component

#endif  // MMORPG_CORE_COMPONENT_ACTOR_COMMAND_HPP
