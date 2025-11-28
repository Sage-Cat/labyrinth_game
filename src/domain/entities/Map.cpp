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
    if (!in_bounds(p))
        return false;
    const auto &tile = tiles_.at(p.x, p.y);
    return !tile.blocks_sight;
}
} // namespace Domain::Entities
