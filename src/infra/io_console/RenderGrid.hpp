#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace Infrastructure::IOConsole {

// A safe grid for UTF-8 glyph strings: each cell stores one glyph string
class RenderGrid {
public:
    RenderGrid() = default;
    RenderGrid(std::size_t w, std::size_t h);

    void resize(std::size_t w, std::size_t h);

    [[nodiscard]] std::size_t width() const noexcept { return w_; }
    [[nodiscard]] std::size_t height() const noexcept { return h_; }

    [[nodiscard]] bool in_bounds(std::size_t x, std::size_t y) const noexcept;

    // Cell access
    [[nodiscard]] const std::string &at(std::size_t x, std::size_t y) const;
    std::string &at(std::size_t x, std::size_t y);

    // Convert to printable lines (each line is concatenation of glyphs).
    [[nodiscard]] std::vector<std::string> to_lines() const;

private:
    std::size_t w_{0};
    std::size_t h_{0};
    std::vector<std::string> cells_{};

    [[nodiscard]] std::size_t index(std::size_t x, std::size_t y) const noexcept
    {
        return y * w_ + x;
    }
};

} // namespace Infrastructure::IOConsole
