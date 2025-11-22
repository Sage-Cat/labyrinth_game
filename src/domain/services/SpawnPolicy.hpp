#pragma once
#include "domain/core/IRng.hpp"
#include <memory>
#include <vector>

namespace Domain::Rules {
struct GameRules;
}
namespace Domain::Entities {
class Map;
class Player;
class Enemy;
class Item;
} // namespace Domain::Entities

namespace Domain::Services {
struct SpawnPolicy {
    static void place_player(const Domain::Rules::GameRules &rules, Domain::Core::IRng &rng,
                             Domain::Entities::Map &map, Domain::Entities::Player &player);

    static void place_enemies(const Domain::Rules::GameRules &rules, Domain::Core::IRng &rng,
                              Domain::Entities::Map &map,
                              std::vector<Domain::Entities::Enemy> &enemies);

    static void place_items(const Domain::Rules::GameRules &rules, Domain::Core::IRng &rng,
                            Domain::Entities::Map &map,
                            std::vector<std::unique_ptr<Domain::Entities::Item>> &items);
};
} // namespace Domain::Services
