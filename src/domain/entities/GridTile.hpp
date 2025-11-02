#pragma once
#include "domain/core/Grid.hpp"
#include "domain/entities/Tile.hpp"

namespace Domain::Entities
{
    // Thin wrapper to match the diagram name "GridTile".
    class GridTile
    {
    public:
        GridTile() = default;
        GridTile(std::size_t w, std::size_t h) : grid_{w, h} {}
        [[nodiscard]] std::size_t width() const { return grid_.width(); }
        [[nodiscard]] std::size_t height() const { return grid_.height(); }

        bool in_bounds(std::size_t x, std::size_t y) const { return grid_.in_bounds(x, y); }
        Tile &at(std::size_t x, std::size_t y) { return grid_.at(x, y); }
        const Tile &at(std::size_t x, std::size_t y) const { return grid_.at(x, y); }

    private:
        Domain::Core::Grid<Tile> grid_{};
    };
}
