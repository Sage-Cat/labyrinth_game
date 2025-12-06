#include "domain/services/MapGenHelpers.hpp"

#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"

#include <algorithm>

namespace Domain::Services::MapGenDetail {

using Domain::Core::Position;
using Domain::Entities::Map;
using Domain::Entities::TileType;

Position Room::center() const noexcept
{
    return Position{
        static_cast<std::uint16_t>(x + w / 2),
        static_cast<std::uint16_t>(y + h / 2),
    };
}

bool Room::intersects(const Room &other) const noexcept
{
    // One-tile padding so rooms are not glued together.
    return !(x + w + 1 <= other.x || other.x + other.w + 1 <= x || y + h + 1 <= other.y ||
             other.y + other.h + 1 <= y);
}

void fill_with_walls(Map &map) noexcept
{
    for (std::size_t y = 0; y < map.height(); ++y) {
        for (std::size_t x = 0; x < map.width(); ++x) {
            auto &tile           = map.grid().at(x, y);
            tile.type            = TileType::Wall;
            tile.blocks_movement = true;
            tile.blocks_sight    = true;
        }
    }
}

void carve_room(Map &map, const Room &room) noexcept
{
    const auto max_x = std::min<std::uint16_t>(static_cast<std::uint16_t>(room.x + room.w),
                                               static_cast<std::uint16_t>(map.width()));
    const auto max_y = std::min<std::uint16_t>(static_cast<std::uint16_t>(room.y + room.h),
                                               static_cast<std::uint16_t>(map.height()));

    for (std::uint16_t y = room.y; y < max_y; ++y) {
        for (std::uint16_t x = room.x; x < max_x; ++x) {
            auto &tile           = map.grid().at(x, y);
            tile.type            = TileType::Floor;
            tile.blocks_movement = false;
            tile.blocks_sight    = false;
        }
    }
}

void carve_h_tunnel(Map &map, std::uint16_t x1, std::uint16_t x2, std::uint16_t y) noexcept
{
    if (map.height() == 0 || map.width() == 0) {
        return;
    }

    if (x1 > x2) {
        std::swap(x1, x2);
    }

    x1 = std::min<std::uint16_t>(x1, static_cast<std::uint16_t>(map.width() - 1));
    x2 = std::min<std::uint16_t>(x2, static_cast<std::uint16_t>(map.width() - 1));
    y  = std::min<std::uint16_t>(y, static_cast<std::uint16_t>(map.height() - 1));

    for (std::uint16_t x = x1; x <= x2; ++x) {
        auto &tile           = map.grid().at(x, y);
        tile.type            = TileType::Floor;
        tile.blocks_movement = false;
        tile.blocks_sight    = false;
    }
}

void carve_v_tunnel(Map &map, std::uint16_t y1, std::uint16_t y2, std::uint16_t x) noexcept
{
    if (map.height() == 0 || map.width() == 0) {
        return;
    }

    if (y1 > y2) {
        std::swap(y1, y2);
    }

    x  = std::min<std::uint16_t>(x, static_cast<std::uint16_t>(map.width() - 1));
    y1 = std::min<std::uint16_t>(y1, static_cast<std::uint16_t>(map.height() - 1));
    y2 = std::min<std::uint16_t>(y2, static_cast<std::uint16_t>(map.height() - 1));

    for (std::uint16_t y = y1; y <= y2; ++y) {
        auto &tile           = map.grid().at(x, y);
        tile.type            = TileType::Floor;
        tile.blocks_movement = false;
        tile.blocks_sight    = false;
    }
}

void enforce_border_walls(Map &map) noexcept
{
    const auto w = map.width();
    const auto h = map.height();

    if (w == 0 || h == 0) {
        return;
    }

    // Top and bottom rows.
    for (std::size_t x = 0; x < w; ++x) {
        auto &top           = map.grid().at(x, 0);
        top.type            = TileType::Wall;
        top.blocks_movement = true;
        top.blocks_sight    = true;

        auto &bottom           = map.grid().at(x, h - 1);
        bottom.type            = TileType::Wall;
        bottom.blocks_movement = true;
        bottom.blocks_sight    = true;
    }

    // Left and right columns.
    for (std::size_t y = 0; y < h; ++y) {
        auto &left           = map.grid().at(0, y);
        left.type            = TileType::Wall;
        left.blocks_movement = true;
        left.blocks_sight    = true;

        auto &right           = map.grid().at(w - 1, y);
        right.type            = TileType::Wall;
        right.blocks_movement = true;
        right.blocks_sight    = true;
    }
}

} // namespace Domain::Services::MapGenDetail
