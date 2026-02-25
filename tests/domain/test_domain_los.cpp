#include <cstdlib>
#include <iostream>

#include "../includes/test_utils.hpp"
#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/services/LOS.hpp"

namespace {
using Domain::Core::Position;
using Domain::Entities::Map;
using Domain::Entities::TileType;
using Domain::Services::LOS;
using TestUtils::expect;
using TestUtils::fail_count;
using TestUtils::reset_map;
} // namespace

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

    reset_map(map);
    map.grid().at(2, 1).blocks_sight = true;
    map.grid().at(1, 2).blocks_sight = true;
    expect(!LOS::can_see(map, Position{1, 1}, Position{2, 2}),
           "Diagonal line blocked by corner walls");

    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}