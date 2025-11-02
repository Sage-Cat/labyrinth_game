#include "domain/services/Pathfinding.hpp"
#include "domain/entities/Map.hpp"

namespace Domain::Services
{
    std::optional<Domain::Core::Direction>
    Pathfinding::step_towards(const Domain::Entities::Map &, Domain::Core::Position, Domain::Core::Position)
    {
        // TODO: find path to Player and step towards
        return std::nullopt; 
    }
}
