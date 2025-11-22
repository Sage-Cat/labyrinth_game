#pragma once
#include <cstdint>
#include <string>

namespace Domain::Rules {
struct GameRules {
    std::uint16_t map_w{20};
    std::uint16_t map_h{20};
    std::uint16_t enemy_count{3};
    int potion_heal_max{5};
    std::string drop_rates_note{}; // textual note/justification
};

inline const GameRules DEFAULT_RULES{};
} // namespace Domain::Rules
