#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"

namespace Domain::Entities {
bool Map::is_passable(Domain::Core::Position p) const
{
    (void)p;
    return false; // TODO: return if is passable
}
bool Map::is_transparent(Domain::Core::Position p) const
{
    (void)p;
    return false; // TODO: return if is transparent (for LOS)
}
} // namespace Domain::Entities
