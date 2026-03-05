#include <cassert>
#include <iostream>
#include <memory>

#include "app/systems/EnemyAISystem.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"

using Domain::Core::Position;
using Domain::Entities::Enemy;
using Domain::Entities::EnemyState;
using Domain::Entities::Player;

int main()
{
    Domain::Core::GameState state{};
    state.map = Domain::Entities::Map(3, 1);

    for (std::uint16_t x = 0; x < state.map.width(); ++x) {
        for (std::uint16_t y = 0; y < state.map.height(); ++y) {
            (void)state.map.set_passable({x, y}, true);
            (void)state.map.set_transparent({x, y}, true);
        }
    }

    auto player = std::make_unique<Player>();
    player->pos = Position{2, 0};

    auto enemy   = std::make_unique<Enemy>();
    enemy->pos   = Position{0, 0};
    enemy->state = EnemyState::Resting;

    state.actors.push_back(std::move(player));
    state.actors.push_back(std::move(enemy));

    auto *enemy_ptr = dynamic_cast<Enemy *>(state.actors[1].get());

    //chasing check
    Application::Systems::EnemyAISystem{}.action(state);
    assert(enemy_ptr->state == EnemyState::Chasing);
    assert(enemy_ptr->pos == (Position{1, 0}));

    //LOS check
    (void)state.map.set_transparent({1, 0}, false);
    Application::Systems::EnemyAISystem{}.action(state);
    assert(enemy_ptr->state == EnemyState::Resting);
    (void)state.map.set_transparent({1, 0}, true);

    //quit check
    auto prev_pos   = enemy_ptr->pos;
    auto prev_state = enemy_ptr->state;
    state.quit      = true;
    Application::Systems::EnemyAISystem{}.action(state);
    assert(enemy_ptr->pos == prev_pos);
    assert(enemy_ptr->state == prev_state);
    (void)prev_pos;
    (void)prev_state;
    state.quit = false;

    //defeat check
    Application::Systems::EnemyAISystem{}.action(state);
    assert(state.defeat == true);

    return 0;
}