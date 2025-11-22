#pragma once
#include "domain/core/Position.hpp"

namespace Domain::Entities {
class Map;
}

namespace Domain::Services {
struct LOS {
    static bool can_see(const Domain::Entities::Map &map, Domain::Core::Position a,
                        Domain::Core::Position b);
};
} // namespace Domain::Services
