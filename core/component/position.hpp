#ifndef MMORPG_CORE_COMPONENT_POSITION_HPP
#define MMORPG_CORE_COMPONENT_POSITION_HPP

#include <limits>

#include <msgpack.hpp>

namespace core::component {

struct Position {
    float x = -std::numeric_limits<float>::infinity();
    float y = -std::numeric_limits<float>::infinity();
    MSGPACK_DEFINE_MAP(x, y)
};

}  // namespace core::component

#endif  // MMORPG_CORE_COMPONENT_POSITION_HPP
