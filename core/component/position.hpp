#ifndef MMORPG_CORE_COMPONENT_POSITION_HPP
#define MMORPG_CORE_COMPONENT_POSITION_HPP

#include <limits>

namespace core::component {

struct Position {
    float x = -std::numeric_limits<float>::infinity();
    float y = -std::numeric_limits<float>::infinity();
};

}  // namespace core::component

#endif  // MMORPG_CORE_COMPONENT_POSITION_HPP
