#pragma once

#include <cstdint>

namespace Domain::Core {
struct Position;
} // namespace Domain::Core

namespace Domain::Entities {
class Map;
} // namespace Domain::Entities

namespace Domain::Services::MapGenDetail {

// Simple rectangular room used by MapGen.
struct Room {
    std::uint16_t x{};
    std::uint16_t y{};
    std::uint16_t w{};
    std::uint16_t h{};

    [[nodiscard]] Domain::Core::Position center() const noexcept;

    // Returns true if this room intersects another room
    // (with 1-tile padding).
    [[nodiscard]] bool intersects(const Room &other) const noexcept;
};

// Helpers
void fill_with_walls(Domain::Entities::Map &map) noexcept;

void carve_room(Domain::Entities::Map &map, const Room &room) noexcept;

void carve_h_tunnel(Domain::Entities::Map &map, std::uint16_t x1, std::uint16_t x2,
                    std::uint16_t y) noexcept;

void carve_v_tunnel(Domain::Entities::Map &map, std::uint16_t y1, std::uint16_t y2,
                    std::uint16_t x) noexcept;

void enforce_border_walls(Domain::Entities::Map &map) noexcept;

} // namespace Domain::Services::MapGenDetail
