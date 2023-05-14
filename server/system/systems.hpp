#ifndef MMORPG_SERVER_SYSTEM_SYSTEMS_HPP
#define MMORPG_SERVER_SYSTEM_SYSTEMS_HPP

#include "flecs.h"

namespace server::system {

extern void print_entities(flecs::world& ecs);

extern void afk_check(flecs::world& ecs);

extern void acknowledge_server_messages(flecs::world& ecs);

}  // namespace server::system

#endif  // MMORPG_SERVER_SYSTEM_SYSTEMS_HPP
