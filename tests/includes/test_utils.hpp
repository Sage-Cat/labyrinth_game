#pragma once
#include <iostream>
#include <string>
#include "domain/entities/Map.hpp"

namespace TestUtils {

inline int fail_count = 0;

inline bool expect(const bool condition, const std::string &name)
{
    if (condition) {
        std::cout << "[PASS]" << " " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL]" << " " << name << "\n";
        fail_count++;
        return false;
    }
}

inline void reset_fail_count() { fail_count = 0; }

void reset_map(Domain::Entities::Map &map)
{
    for (std::size_t x = 0; x < map.width(); ++x) {
        for (std::size_t y = 0; y < map.height(); ++y) {
            auto &tile           = map.grid().at(x, y);
            tile.type            = Domain::Entities::TileType::Floor;
            tile.blocks_sight    = false;
            tile.blocks_movement = false;
        }
    }
}

} // namespace TestUtils