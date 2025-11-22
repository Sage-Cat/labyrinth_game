#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include "domain/core/IRng.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/rules/GameRules.hpp"

namespace Domain::Core {
struct GameState {
    // world data
    Domain::Entities::Map map{};
    std::vector<std::unique_ptr<Domain::Entities::Actor>> actors{};
    std::vector<std::unique_ptr<Domain::Entities::Item>> items{};

    // progression
    std::uint64_t turn{0};
    bool victory{false};
    bool defeat{false};
    int score{0};

    // config & services
    Domain::Rules::GameRules rules{Domain::Rules::DEFAULT_RULES};
    IRng *rng{nullptr}; // owned elsewhere; GameState depends on it
};
} // namespace Domain::Core
