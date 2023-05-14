#ifndef MMORPG_CORE_SYSTEM_SYSTEMS_HPP
#define MMORPG_CORE_SYSTEM_SYSTEMS_HPP

#include "flecs.h"

namespace core::system {

// Updates based on input components (character command)
extern void update_velocity(flecs::world& ecs, flecs::entity_t stage);

// Clears character command
extern void clear_actor_command(flecs::world& ecs, flecs::entity_t stage);

// Updates based on non-input components
extern void update_position(flecs::world& ecs, flecs::entity_t stage);

}  // namespace core::system

#endif  // MMORPG_CORE_SYSTEM_SYSTEMS_HPP
