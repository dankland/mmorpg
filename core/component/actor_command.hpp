#ifndef MMORPG_CORE_COMPONENT_ACTOR_COMMAND_HPP
#define MMORPG_CORE_COMPONENT_ACTOR_COMMAND_HPP

#include <array>
#include <cstdint>

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
    std::uint64_t m_commands = Type::None;
};

class ActorCommandBuffer {
   public:
    static constexpr std::size_t size = 12;

    ActorCommandBuffer() : m_command_buffer{} {
    }

    void push(ActorCommand command) {
        m_command_buffer[m_tail] = command;
        m_tail = (m_tail + 1) % size;
    }
    [[nodiscard]] ActorCommand pop() {
        ActorCommand command = m_command_buffer[m_head];
        m_head = (m_head + 1) % size;
        return command;
    }
    [[nodiscard]] bool empty() const {
        return m_head == m_tail;
    }
    [[nodiscard]] bool full() const {
        return (m_tail + 1) % size == m_head;
    }

   private:
    std::size_t m_head = 0;
    std::size_t m_tail = 0;
    std::array<ActorCommand, size> m_command_buffer;
};

}  // namespace core::component

#endif  // MMORPG_CORE_COMPONENT_ACTOR_COMMAND_HPP
