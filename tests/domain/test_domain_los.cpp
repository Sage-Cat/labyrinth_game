#include <cstdlib>
#include <iostream>

#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/services/LOS.hpp"
#include "test_utils.hpp"

namespace {
using Domain::Core::Position;
using Domain::Entities::Map;
using Domain::Entities::TileType;
using Domain::Services::LOS;
using TestUtils::expect;
using TestUtils::fail_count;
} // namespace

void reset_map(Map &map)
{
    for (std::size_t x = 0; x < map.width(); ++x) {
        for (std::size_t y = 0; y < map.height(); ++y) {
            auto &tile           = map.grid().at(x, y);
            tile.type            = TileType::Floor;
            tile.blocks_sight    = false;
            tile.blocks_movement = false;
        }
    }
}

int main()
{
    Map map{20, 20};

    reset_map(map);
    expect(LOS::can_see(map, Position{1, 1}, Position{5, 1}), "Straight line visible");

    reset_map(map);
    map.grid().at(3, 1).blocks_sight = true;
    expect(!LOS::can_see(map, Position{1, 1}, Position{5, 1}), "Straight line blocked by wall");

    reset_map(map);
    expect(LOS::can_see(map, Position{1, 1}, Position{5, 5}), "Diagonal line visible");

    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}