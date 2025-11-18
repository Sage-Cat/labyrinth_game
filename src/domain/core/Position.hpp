#pragma once
#include <cstdint>

namespace Domain::Core {
struct Position {
    std::uint16_t x{};
    std::uint16_t y{};
};
} // namespace Domain::Core
