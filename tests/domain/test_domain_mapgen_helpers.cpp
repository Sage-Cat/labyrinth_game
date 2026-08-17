#include <cstdlib>
#include <iostream>

#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/services/MapGenHelpers.hpp"

#include "test_utils.hpp"

namespace {
using Domain::Core::Position;
using Domain::Entities::Map;
using Domain::Entities::TileType;
using Domain::Services::MapGenDetail::Room;
using namespace Domain::Services::MapGenDetail;

using TestUtils::expect;
using TestUtils::fail_count;

// Helper used only in tests to set everything to floor.
void fill_with_floors(Map &map)
{
    for (std::size_t y = 0; y < map.height(); ++y) {
        for (std::size_t x = 0; x < map.width(); ++x) {
            auto &tile           = map.grid().at(x, y);
            tile.type            = TileType::Floor;
            tile.blocks_movement = false;
            tile.blocks_sight    = false;
        }
    }
}

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
    // --- Room::center ---
    {
        Room room{2, 4, 6, 4}; // x=[2..7], y=[4..7]
        const auto center = room.center();
        expect(center.x == 2 + 6 / 2 && center.y == 4 + 4 / 2,
               "Room::center returns geometric center");
    }

    // --- Room::intersects ---
    {
        Room a{2, 2, 4, 4};
        Room b{5, 2, 4, 4}; // touches with 1-tile padding?
        Room c{10, 10, 3, 3};

        expect(a.intersects(a), "Room intersects itself");
        expect(a.intersects(b) || b.intersects(a),
               "Rooms close to each other intersect with padding");
        expect(!a.intersects(c) && !c.intersects(a), "Distant rooms do not intersect");
    }

    Map map{20, 15};

    // --- fill_with_walls ---
    {
        fill_with_floors(map); // start from non-wall state
        fill_with_walls(map);

        bool all_walls = true;
        for (std::size_t y = 0; y < map.height(); ++y) {
            for (std::size_t x = 0; x < map.width(); ++x) {
                if (!is_wall(map, x, y)) {
                    all_walls = false;
                    break;
                }
            }
            if (!all_walls) {
                break;
            }
        }

        expect(all_walls, "fill_with_walls makes all tiles walls");
    }

    // --- carve_room ---
    {
        fill_with_walls(map);

        const Room room{2, 3, 5, 4}; // x=[2..6], y=[3..6]
        carve_room(map, room);

        bool room_all_floor   = true;
        bool outside_has_wall = false;

        for (std::size_t y = 0; y < map.height(); ++y) {
            for (std::size_t x = 0; x < map.width(); ++x) {
                const bool inside =
                    x >= room.x && x < room.x + room.w && y >= room.y && y < room.y + room.h;

                if (inside) {
                    if (!is_floor(map, x, y)) {
                        room_all_floor = false;
                    }
                } else {
                    if (is_wall(map, x, y)) {
                        outside_has_wall = true;
                    }
                }
            }
        }

        expect(room_all_floor, "carve_room makes all room tiles floors");
        expect(outside_has_wall, "carve_room keeps at least some outside tiles as walls");
    }

    // --- carve_h_tunnel ---
    {
        fill_with_walls(map);

        const std::uint16_t y = 7;
        carve_h_tunnel(map, 3, 10, y);

        bool all_tunnel_floor = true;
        for (std::size_t x = 3; x <= 10; ++x) {
            if (!is_floor(map, x, y)) {
                all_tunnel_floor = false;
                break;
            }
        }

        bool wall_outside = is_wall(map, 0, y) && is_wall(map, map.width() - 1, y);

        expect(all_tunnel_floor, "carve_h_tunnel carves continuous horizontal floor");
        expect(wall_outside, "carve_h_tunnel does not touch far outside tiles");
    }

    // --- carve_v_tunnel ---
    {
        fill_with_walls(map);

        const std::uint16_t x = 5;
        carve_v_tunnel(map, 2, 9, x);

        bool all_tunnel_floor = true;
        for (std::size_t y = 2; y <= 9; ++y) {
            if (!is_floor(map, x, y)) {
                all_tunnel_floor = false;
                break;
            }
        }

        bool wall_outside = is_wall(map, x, 0) && is_wall(map, x, map.height() - 1);

        expect(all_tunnel_floor, "carve_v_tunnel carves continuous vertical floor");
        expect(wall_outside, "carve_v_tunnel does not touch far outside tiles");
    }

    // --- enforce_border_walls ---
    {
        fill_with_floors(map);
        enforce_border_walls(map);

        bool borders_all_walls  = true;
        bool interior_has_floor = false;

        const auto w = map.width();
        const auto h = map.height();

        for (std::size_t x = 0; x < w; ++x) {
            if (!is_wall(map, x, 0) || !is_wall(map, x, h - 1)) {
                borders_all_walls = false;
                break;
            }
        }
        for (std::size_t y = 0; y < h; ++y) {
            if (!is_wall(map, 0, y) || !is_wall(map, w - 1, y)) {
                borders_all_walls = false;
                break;
            }
        }

        for (std::size_t y = 1; y + 1 < h; ++y) {
            for (std::size_t x = 1; x + 1 < w; ++x) {
                if (is_floor(map, x, y)) {
                    interior_has_floor = true;
                    break;
                }
            }
            if (interior_has_floor) {
                break;
            }
        }

        expect(borders_all_walls, "enforce_border_walls sets all border tiles to walls");
        expect(interior_has_floor,
               "enforce_border_walls keeps interior unaffected (still has floors)");
    }

    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
