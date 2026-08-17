#pragma once
#include <cstdint>
#include <cstdlib>

namespace Domain::Core {

struct Position {
    std::uint16_t x{};
    std::uint16_t y{};

    friend constexpr bool operator==(Position, Position) noexcept = default;
};

inline bool is_adjacent(Position a, Position b)
{
    const int dx = std::abs(static_cast<int>(a.x) - static_cast<int>(b.x));
    const int dy = std::abs(static_cast<int>(a.y) - static_cast<int>(b.y));

    return dx + dy == 1;
}
} // namespace Domain::Core
