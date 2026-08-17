#include "infra/io_console/RenderGrid.hpp"

#include <stdexcept>

namespace Infrastructure::IOConsole {

RenderGrid::RenderGrid(std::size_t w, std::size_t h) { resize(w, h); }

void RenderGrid::resize(std::size_t w, std::size_t h)
{
    w_ = w;
    h_ = h;
    cells_.assign(w_ * h_, " ");
}

bool RenderGrid::in_bounds(std::size_t x, std::size_t y) const noexcept { return x < w_ && y < h_; }

const std::string &RenderGrid::at(std::size_t x, std::size_t y) const
{
    if (!in_bounds(x, y)) {
        throw std::out_of_range("RenderGrid::at out of bounds");
    }
    return cells_[index(x, y)];
}

std::string &RenderGrid::at(std::size_t x, std::size_t y)
{
    if (!in_bounds(x, y)) {
        throw std::out_of_range("RenderGrid::at out of bounds");
    }
    return cells_[index(x, y)];
}

std::vector<std::string> RenderGrid::to_lines() const
{
    std::vector<std::string> lines;
    lines.reserve(h_);

    for (std::size_t y = 0; y < h_; ++y) {
        std::string row;
        row.reserve(w_);
        for (std::size_t x = 0; x < w_; ++x) {
            row += cells_[index(x, y)];
        }
        lines.push_back(std::move(row));
    }

    return lines;
}

} // namespace Infrastructure::IOConsole
