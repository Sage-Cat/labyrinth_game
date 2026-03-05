#pragma once
#include <cstdint>

namespace Domain::Core {

struct Position {
    std::uint16_t x{};
    std::uint16_t y{};

    friend constexpr bool operator==(Position, Position) noexcept = default;
};
} // namespace Domain::Core
