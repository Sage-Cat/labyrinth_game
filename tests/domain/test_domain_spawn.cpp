#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "domain/core/IRng.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/services/SpawnPolicy.hpp"

#include "dummy_rng.hpp"

int main()
{
    Domain::Rules::GameRules rules{};
    Domain::Entities::Map map{rules.map_w, rules.map_h};
    Domain::Entities::Player player{};
    std::vector<Domain::Entities::Enemy> enemies;
    std::vector<std::unique_ptr<Domain::Entities::Item>> items;

    DummyRng rng{};
    Domain::Services::SpawnPolicy::place_player(rules, rng, map, player);
    Domain::Services::SpawnPolicy::place_enemies(rules, rng, map, enemies);
    Domain::Services::SpawnPolicy::place_items(rules, rng, map, items);

    std::cout << "[Spawn] player=(" << int(player.pos.x) << "," << int(player.pos.y)
              << "), enemies=" << enemies.size() << ", items=" << items.size() << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
