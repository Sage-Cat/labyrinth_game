#include <cstdlib>
#include <iostream>

#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/services/LOS.hpp"
#include "../includes/test_utils.hpp"

namespace {
using Domain::Core::Position;
using Domain::Entities::Map;
using Domain::Entities::TileType;
using Domain::Services::LOS;
using TestUtils::expect;
using TestUtils::reset_map;
using TestUtils::fail_count;
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

    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}