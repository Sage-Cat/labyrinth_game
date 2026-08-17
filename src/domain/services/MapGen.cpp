#include "domain/services/MapGen.hpp"

#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/entities/TileType.hpp"
#include "domain/rules/GameRules.hpp"

#include "infra/log/Logger.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Domain::Services
{
namespace
{
    constexpr std::uint16_t kMinMapSide = 5;
    constexpr std::uint16_t kMaxMapSide = 255;
    constexpr std::uint16_t kMinRoomSide = 3;

    std::uint16_t clamp_map_side(std::uint16_t side) noexcept
    {
        return std::clamp(side, kMinMapSide, kMaxMapSide);
    }

    void make_wall(Domain::Entities::Map &map, std::size_t x, std::size_t y)
    {
        auto &tile           = map.grid().at(x, y);
        tile.type            = Domain::Entities::TileType::Wall;
        tile.blocks_movement = true;
        tile.blocks_sight    = true;
    }

    void make_floor(Domain::Entities::Map &map, std::size_t x, std::size_t y)
    {
        auto &tile           = map.grid().at(x, y);
        tile.type            = Domain::Entities::TileType::Floor;
        tile.blocks_movement = false;
        tile.blocks_sight    = false;
    }

    void carve_room(Domain::Entities::Map &map, std::uint16_t x, std::uint16_t y, std::uint16_t w,
                    std::uint16_t h)
    {
        const std::uint16_t x_end = static_cast<std::uint16_t>(x + w);
        const std::uint16_t y_end = static_cast<std::uint16_t>(y + h);

        for (std::uint16_t yy = y; yy < y_end; ++yy) {
            for (std::uint16_t xx = x; xx < x_end; ++xx) {
                make_floor(map, xx, yy);
            }
        }
    }

    Domain::Core::Position room_center(std::uint16_t x, std::uint16_t y, std::uint16_t w,
                                       std::uint16_t h)
    {
        return Domain::Core::Position{
            static_cast<std::uint16_t>(x + (w / 2)),
            static_cast<std::uint16_t>(y + (h / 2)),
        };
    }

    void carve_h_corridor(Domain::Entities::Map &map, std::uint16_t x0, std::uint16_t x1,
                          std::uint16_t y)
    {
        std::uint16_t x_start = x0;
        std::uint16_t x_end   = x1;
        if (x_start > x_end) {
            std::swap(x_start, x_end);
        }

        for (std::uint16_t x = x_start; x <= x_end; ++x) {
            make_floor(map, x, y);
        }
    }

    void carve_v_corridor(Domain::Entities::Map &map, std::uint16_t y0, std::uint16_t y1,
                          std::uint16_t x)
    {
        std::uint16_t y_start = y0;
        std::uint16_t y_end   = y1;
        if (y_start > y_end) {
            std::swap(y_start, y_end);
        }

        for (std::uint16_t y = y_start; y <= y_end; ++y) {
            make_floor(map, x, y);
        }
    }

    std::size_t count_floor_tiles(const Domain::Entities::Map &map)
    {
        std::size_t floor_count = 0;
        for (std::uint16_t y = 0; y < map.height(); ++y) {
            for (std::uint16_t x = 0; x < map.width(); ++x) {
                if (!map.grid().at(x, y).blocks_movement) {
                    ++floor_count;
                }
            }
        }
        return floor_count;
    }
} // namespace

void MapGen::generate(const Domain::Rules::GameRules &rules, Domain::Core::IRng &rng,
                      Domain::Entities::Map &map)
{
    const std::uint16_t width  = clamp_map_side(rules.map_w);
    const std::uint16_t height = clamp_map_side(rules.map_h);
    LOG(INFO) << "MapGen::generate started, requested=" << rules.map_w << "x" << rules.map_h
              << ", clamped=" << width << "x" << height;

    map = Domain::Entities::Map{width, height};

    for (std::uint16_t y = 0; y < height; ++y) {
        for (std::uint16_t x = 0; x < width; ++x) {
            make_wall(map, x, y);
        }
    }

    const std::uint16_t max_room_side_w =
        std::max<std::uint16_t>(kMinRoomSide, static_cast<std::uint16_t>(width / 3));
    const std::uint16_t max_room_side_h =
        std::max<std::uint16_t>(kMinRoomSide, static_cast<std::uint16_t>(height / 3));
    const std::uint16_t room_side_w_cap = std::min<std::uint16_t>(
        10, std::min<std::uint16_t>(max_room_side_w, static_cast<std::uint16_t>(width - 2)));
    const std::uint16_t room_side_h_cap = std::min<std::uint16_t>(
        8, std::min<std::uint16_t>(max_room_side_h, static_cast<std::uint16_t>(height - 2)));

    std::vector<Domain::Core::Position> centers;
    constexpr std::uint16_t kRoomAttempts = 12;

    for (std::uint16_t i = 0; i < kRoomAttempts; ++i) {
        if (room_side_w_cap < kMinRoomSide || room_side_h_cap < kMinRoomSide) {
            break;
        }

        const std::uint16_t room_w =
            static_cast<std::uint16_t>(rng.next_int(kMinRoomSide, room_side_w_cap));
        const std::uint16_t room_h =
            static_cast<std::uint16_t>(rng.next_int(kMinRoomSide, room_side_h_cap));

        if (room_w >= width - 1 || room_h >= height - 1) {
            continue;
        }

        const int max_x = static_cast<int>(width - room_w - 1);
        const int max_y = static_cast<int>(height - room_h - 1);
        if (max_x < 1 || max_y < 1) {
            continue;
        }

        const std::uint16_t room_x = static_cast<std::uint16_t>(rng.next_int(1, max_x));
        const std::uint16_t room_y = static_cast<std::uint16_t>(rng.next_int(1, max_y));
        carve_room(map, room_x, room_y, room_w, room_h);

        const auto center = room_center(room_x, room_y, room_w, room_h);
        if (!centers.empty()) {
            const auto prev_center = centers.back();
            const bool horizontal_first = rng.next_int(0, 1) == 0;

            if (horizontal_first) {
                carve_h_corridor(map, prev_center.x, center.x, prev_center.y);
                carve_v_corridor(map, prev_center.y, center.y, center.x);
            } else {
                carve_v_corridor(map, prev_center.y, center.y, prev_center.x);
                carve_h_corridor(map, prev_center.x, center.x, center.y);
            }
        }
        centers.push_back(center);
    }

    // Keep a solid border so the map remains visually closed.
    for (std::uint16_t x = 0; x < width; ++x) {
        make_wall(map, x, 0);
        make_wall(map, x, static_cast<std::uint16_t>(height - 1));
    }
    for (std::uint16_t y = 0; y < height; ++y) {
        make_wall(map, 0, y);
        make_wall(map, static_cast<std::uint16_t>(width - 1), y);
    }

    if (count_floor_tiles(map) == 0) {
        // Safety fallback: carve the inner area to keep map non-empty.
        for (std::uint16_t y = 1; y < static_cast<std::uint16_t>(height - 1); ++y) {
            for (std::uint16_t x = 1; x < static_cast<std::uint16_t>(width - 1); ++x) {
                make_floor(map, x, y);
            }
        }
    }

    LOG(INFO) << "MapGen::generate finished, rooms=" << centers.size()
              << ", floor_tiles=" << count_floor_tiles(map);
}
} // namespace Domain::Services
