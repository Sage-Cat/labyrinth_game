#pragma once
#include <cstdint>
#include "domain/core/Position.hpp"
#include "domain/entities/GridTile.hpp"

namespace Domain::Entities
{
    class Map
    {
    public:
        Map() = default;
        Map(std::uint16_t w, std::uint16_t h) : tiles_{w, h} {}

        [[nodiscard]] std::uint16_t width() const { return static_cast<std::uint16_t>(tiles_.width()); }
        [[nodiscard]] std::uint16_t height() const { return static_cast<std::uint16_t>(tiles_.height()); }

        [[nodiscard]] bool in_bounds(Domain::Core::Position p) const
        {
            return tiles_.in_bounds(p.x, p.y);
        }
        [[nodiscard]] bool is_passable(Domain::Core::Position p) const;
        [[nodiscard]] bool is_transparent(Domain::Core::Position p) const;

        // Access (optional for future systems)
        GridTile &grid() { return tiles_; }
        const GridTile &grid() const { return tiles_; }

    private:
        GridTile tiles_{};
    };
}
