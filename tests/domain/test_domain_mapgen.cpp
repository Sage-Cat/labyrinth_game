#include <cstdlib>
#include <iostream>

#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/services/MapGen.hpp"

#include "dummy_rng.hpp"
#include "test_utils.hpp"

namespace {
using Domain::Entities::Map;
using Domain::Entities::TileType;
using Domain::Rules::GameRules;
using Domain::Services::MapGen;

using TestUtils::expect;
using TestUtils::fail_count;

bool is_wall(const Map &map, std::size_t x, std::size_t y)
{
    const auto &tile = map.grid().at(x, y);
    return tile.type == TileType::Wall && tile.blocks_movement && tile.blocks_sight;
}

bool is_floor(const Map &map, std::size_t x, std::size_t y)
{
    const auto &tile = map.grid().at(x, y);
    return tile.type == TileType::Floor && !tile.blocks_movement && !tile.blocks_sight;
}

} // namespace

int main()
{
    GameRules rules{};
    // Use explicit values to make the test intent clear.
    rules.map_w         = 40;
    rules.map_h         = 25;
    rules.max_rooms     = 8;
    rules.room_min_size = 4;
    rules.room_max_size = 8;

    DummyRng rng{};
    Map map{rules.map_w, rules.map_h};

    MapGen::generate(rules, rng, map);

    std::cout << "[MapGen] generated map: " << map.width() << "x" << map.height() << "\n";

    // 1) Dimensions must match GameRules (with minimal clamp already applied inside).
    expect(map.width() == rules.map_w, "MapGen: width matches GameRules.map_w");
    expect(map.height() == rules.map_h, "MapGen: height matches GameRules.map_h");

    // 2) There must be at least one floor tile.
    std::size_t floor_count = 0;
    for (std::size_t y = 0; y < map.height(); ++y) {
        for (std::size_t x = 0; x < map.width(); ++x) {
            if (is_floor(map, x, y)) {
                ++floor_count;
            }
        }
    }

    expect(floor_count > 0, "MapGen: at least one floor tile exists");
    expect(floor_count < map.width() * map.height(),
           "MapGen: map is not fully carved (some walls remain)");

    // 3) Borders must be walls.
    bool borders_all_walls = true;

    const auto w = map.width();
    const auto h = map.height();

    // Top and bottom rows.
    for (std::size_t x = 0; x < w; ++x) {
        if (!is_wall(map, x, 0) || !is_wall(map, x, h - 1)) {
            borders_all_walls = false;
            break;
        }
    }

    // Left and right columns.
    if (borders_all_walls) {
        for (std::size_t y = 0; y < h; ++y) {
            if (!is_wall(map, 0, y) || !is_wall(map, w - 1, y)) {
                borders_all_walls = false;
                break;
            }
        }
    }

    expect(borders_all_walls, "MapGen: borders are solid walls");

    // Simple sanity: there is at least one floor not on border.
    bool interior_floor_exists = false;
    for (std::size_t y = 1; y + 1 < h && !interior_floor_exists; ++y) {
        for (std::size_t x = 1; x + 1 < w; ++x) {
            if (is_floor(map, x, y)) {
                interior_floor_exists = true;
                break;
            }
        }
    }
    expect(interior_floor_exists, "MapGen: at least one interior floor tile exists");

    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
