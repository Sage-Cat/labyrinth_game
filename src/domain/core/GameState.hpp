#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include "domain/core/AttackIntent.hpp"
#include "domain/core/IRng.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/rules/GameRules.hpp"

namespace Domain::Core {
struct GameState {
    // world data
    Domain::Entities::Map map{};
    const Domain::Entities::Actor *find_actor(EntityId id) const
    {
        for (const auto &a : actors) {
            if (a && a->id == id)
                return a.get();
        }
        return nullptr;
    }
    Domain::Entities::Actor *find_actor(EntityId id)
    {
        for (auto &a : actors) {
            if (a && a->id == id)
                return a.get();
        }
        return nullptr;
    }
    std::vector<std::unique_ptr<Domain::Entities::Actor>> actors{};
    std::vector<std::unique_ptr<Domain::Entities::Item>> items{};
    std::vector<Domain::Core::AttackIntent> intents{};

    // progression
    std::uint64_t turn{0};
    bool victory{false};
    bool defeat{false};
    bool quit{false};
    int score{0};

    // config & services
    Domain::Rules::GameRules rules{Domain::Rules::DEFAULT_RULES};
    IRng *rng{nullptr}; // owned elsewhere; GameState depends on it
};
} // namespace Domain::Core
