#ifndef MMORPG_SERVER_SYSTEM_SYSTEMS_HPP
#define MMORPG_SERVER_SYSTEM_SYSTEMS_HPP

#include "flecs.h"

namespace server::system {

extern void print_entities(flecs::world& ecs);

extern void afk_check(flecs::world& ecs);

extern void acknowledge_server_messages(flecs::world& ecs);

extern void accumulate_actor_commands_for_propagation(flecs::world& ecs);
extern void accumulate_actor_states_for_propagation(flecs::world& ecs);
extern void propagate_accumulated_server_messages(flecs::world& ecs);

}  // namespace server::system

#endif  // MMORPG_SERVER_SYSTEM_SYSTEMS_HPP
