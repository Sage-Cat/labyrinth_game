#pragma once
#include <optional>
#include "domain/core/Direction.hpp"
#include "domain/core/Position.hpp"

namespace Domain::Entities
{
    class Map;
}

namespace Domain::Services
{
    struct Pathfinding
    {
        static std::optional<Domain::Core::Direction>
        step_towards(const Domain::Entities::Map &map,
                     Domain::Core::Position from,
                     Domain::Core::Position to);
    };
}
