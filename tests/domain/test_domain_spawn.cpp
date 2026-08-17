#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/services/MapGen.hpp"
#include "domain/services/SpawnPolicy.hpp"

#include "dummy_rng.hpp"

int main()
{
    Domain::Rules::GameRules rules{};
    Domain::Entities::Map map{rules.map_w, rules.map_h};
    DummyRng rng{};

    Domain::Services::MapGen::generate(rules, rng, map);

    Domain::Entities::Player player{};
    std::vector<Domain::Entities::Enemy> enemies;
    std::vector<std::unique_ptr<Domain::Entities::Item>> items;

    Domain::Services::SpawnPolicy::place_player(rules, rng, map, player);
    Domain::Services::SpawnPolicy::place_enemies(rules, rng, map, enemies);
    Domain::Services::SpawnPolicy::place_items(rules, rng, map, items);

    assert(map.in_bounds(player.pos));
    assert(map.is_passable(player.pos));
    assert(enemies.size() == rules.enemy_count);
    assert(!items.empty());

    for (const auto &enemy : enemies) {
        assert(map.in_bounds(enemy.pos));
        assert(map.is_passable(enemy.pos));
    }

    bool has_key = false;
    for (const auto &item : items) {
        assert(map.in_bounds(item->pos));
        assert(map.is_passable(item->pos));
        has_key = has_key || (dynamic_cast<const Domain::Entities::Key *>(item.get()) != nullptr);
    }
    assert(has_key);

    return 0;
}
