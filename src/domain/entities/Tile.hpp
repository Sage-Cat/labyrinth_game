#pragma once
#include "domain/entities/TileType.hpp"

namespace Domain::Entities
{
    struct Tile
    {
        TileType type{TileType::Wall};
        bool blocks_movement{true};
        bool blocks_sight{true};
    };
}
