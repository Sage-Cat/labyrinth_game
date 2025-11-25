#pragma once
#include <cstdint>

namespace Domain::Core {
struct Position {
    std::uint8_t x;
    std::uint8_t y;

    Position() : x(0), y(0) {}
    Position(std::uint8_t xx, std::uint8_t yy) : x(xx), y(yy) {}
};
} // namespace Domain::Core
