#include "domain/services/MapGen.hpp"

#include "domain/core/IRng.hpp"
#include "domain/entities/Map.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/services/MapGenHelpers.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace Domain::Services {

namespace {

using Domain::Core::IRng;
using Domain::Entities::Map;
using Domain::Rules::GameRules;
using namespace Domain::Services::MapGenDetail;

constexpr std::uint16_t kMinMapSize = 5;

[[nodiscard]] std::uint16_t clamp_size(std::uint16_t value) noexcept
{
    return std::max<std::uint16_t>(value, kMinMapSize);
}

[[nodiscard]] std::int32_t rng_int(IRng &rng, std::int32_t min, std::int32_t max)
{
    // Contract: IRng::next_int(min, max) returns int in [min, max].
    return rng.next_int(min, max);
}

} // namespace

void MapGen::generate(const GameRules &rules, IRng &rng, Map &map)
{
    // Dungeon generation theory https://github.com/Sage-Cat/labyrinth_game/wiki/MapGen-theory

    // 1) Determine map size (with a minimal safety clamp).
    const auto width  = clamp_size(rules.map_w);
    const auto height = clamp_size(rules.map_h);

    // Recreate the map with requested size.
    map = Map{width, height};

    // 2) Fill everything with walls.
    fill_with_walls(map);

    // MapGen parameters (expected in GameRules).
    const std::uint16_t max_rooms     = rules.max_rooms;
    const std::uint16_t room_min_size = rules.room_min_size;
    const std::uint16_t room_max_size = rules.room_max_size;

    std::vector<Room> rooms;
    rooms.reserve(max_rooms);

    // 3) Try to place up to max_rooms non-overlapping rooms.
    for (std::uint16_t i = 0; i < max_rooms; ++i) {
        const auto room_w = static_cast<std::uint16_t>(rng_int(rng, room_min_size, room_max_size));
        const auto room_h = static_cast<std::uint16_t>(rng_int(rng, room_min_size, room_max_size));

        // Skip if room is too large for this map.
        if (room_w >= width - 2 || room_h >= height - 2) {
            continue;
        }

        const auto max_x = static_cast<std::int32_t>(width - room_w - 1);
        const auto max_y = static_cast<std::int32_t>(height - room_h - 1);

        if (max_x <= 1 || max_y <= 1) {
            // Map is too small for additional rooms.
            break;
        }

        const auto x = static_cast<std::uint16_t>(rng_int(rng, 1, max_x));
        const auto y = static_cast<std::uint16_t>(rng_int(rng, 1, max_y));

        Room new_room{x, y, room_w, room_h};

        bool overlaps = false;
        for (const auto &existing : rooms) {
            if (new_room.intersects(existing)) {
                overlaps = true;
                break;
            }
        }

        if (overlaps) {
            continue;
        }

        // 4) Carve the room.
        carve_room(map, new_room);

        // 5) Connect to previous room with a corridor.
        if (!rooms.empty()) {
            const auto prev_center = rooms.back().center();
            const auto new_center  = new_room.center();

            const bool horizontal_first = rng_int(rng, 0, 1) == 0;

            if (horizontal_first) {
                carve_h_tunnel(map, static_cast<std::uint16_t>(prev_center.x),
                               static_cast<std::uint16_t>(new_center.x),
                               static_cast<std::uint16_t>(prev_center.y));
                carve_v_tunnel(map, static_cast<std::uint16_t>(prev_center.y),
                               static_cast<std::uint16_t>(new_center.y),
                               static_cast<std::uint16_t>(new_center.x));
            } else {
                carve_v_tunnel(map, static_cast<std::uint16_t>(prev_center.y),
                               static_cast<std::uint16_t>(new_center.y),
                               static_cast<std::uint16_t>(prev_center.x));
                carve_h_tunnel(map, static_cast<std::uint16_t>(prev_center.x),
                               static_cast<std::uint16_t>(new_center.x),
                               static_cast<std::uint16_t>(new_center.y));
            }
        }

        rooms.push_back(new_room);
    }

    // 6) Ensure outer border is solid walls.
    enforce_border_walls(map);
}

} // namespace Domain::Services
