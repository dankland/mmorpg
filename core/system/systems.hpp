#ifndef MMORPG_CORE_SYSTEM_SYSTEMS_HPP
#define MMORPG_CORE_SYSTEM_SYSTEMS_HPP

#include "flecs.h"

namespace core::system {

extern void load_actor_command(flecs::world& ecs, flecs::entity_t stage);

extern void update_velocity(flecs::world& ecs, flecs::entity_t stage);

extern void update_position(flecs::world& ecs, flecs::entity_t stage);

}  // namespace core::system

#endif  // MMORPG_CORE_SYSTEM_SYSTEMS_HPP
