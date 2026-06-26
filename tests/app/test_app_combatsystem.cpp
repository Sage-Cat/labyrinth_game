#include <cassert>
#include <memory>

#include "app/systems/CombatSystem.hpp"
#include "domain/core/AttackIntent.hpp"
#include "domain/core/GameState.hpp"

#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"

int main()
{
    using namespace Domain::Core;
    using namespace Domain::Entities;
    using namespace Application::Systems;

    GameState state;

    auto player      = std::make_unique<Player>();
    player->id       = EntityId{1};
    player->stats.hp = 10;

    auto enemy      = std::make_unique<Enemy>();
    enemy->id       = EntityId{2};
    enemy->stats.hp = 5;

    auto player_id = player->id;
    auto enemy_id  = enemy->id;

    state.actors.push_back(std::move(player));
    state.actors.push_back(std::move(enemy));

    state.intents.push_back({player_id, enemy_id, 3});

    CombatSystem::resolve(state);

    auto *e = state.find_actor(enemy_id);
    assert(e != nullptr);
    assert(e->stats.hp == 2);

    GameState state2;

    auto player2      = std::make_unique<Player>();
    player2->id       = EntityId{1};
    player2->stats.hp = 3;

    auto enemy2      = std::make_unique<Enemy>();
    enemy2->id       = EntityId{2};
    enemy2->stats.hp = 10;

    auto player2_id = player2->id;
    auto enemy2_id  = enemy2->id;

    state2.actors.push_back(std::move(player2));
    state2.actors.push_back(std::move(enemy2));

    state2.intents.push_back({enemy2_id, player2_id, 3});

    CombatSystem::resolve(state2);

    auto *p = state2.find_actor(player2_id);
    assert(p != nullptr);
    assert(p->stats.hp == 0);
    assert(state2.defeat == true);

    return 0;
}