#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"

namespace Domain::Entities {
bool Map::is_passable(Domain::Core::Position p) const
{
    if (!in_bounds(p))
        return false;
    const auto &tile = tiles_.at(p.x, p.y);
    return !tile.blocks_movement;
}

bool Map::is_transparent(Domain::Core::Position p) const
{
    if (!in_bounds(p))
        return false;
    const auto &tile = tiles_.at(p.x, p.y);
    return !tile.blocks_sight;
}

bool Map::set_passable(Domain::Core::Position p, bool passable)
{
    if (!in_bounds(p))
        return false;
    tiles_.at(p.x, p.y).blocks_movement = !passable;
    return true;
}
} // namespace Domain::Entities
