#include "app/systems/EnemyAISystem.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/services/LOS.hpp"
#include "domain/services/Pathfinding.hpp"
#include "infra/log/Logger.hpp"
#include <cmath>

namespace Application::Systems {

static Domain::Core::Position moved(Domain::Core::Position p, Domain::Core::Direction d)
{
    switch (d) {
    case Domain::Core::Direction::Up:
        return {p.x, static_cast<std::uint16_t>(p.y - 1)};
    case Domain::Core::Direction::Down:
        return {p.x, static_cast<std::uint16_t>(p.y + 1)};
    case Domain::Core::Direction::Left:
        return {static_cast<std::uint16_t>(p.x - 1), p.y};
    case Domain::Core::Direction::Right:
        return {static_cast<std::uint16_t>(p.x + 1), p.y};
    }
    return p;
}

static bool occupied_by_enemy(const Domain::Core::GameState &state, Domain::Core::Position p,
                              const Domain::Entities::Enemy *self)
{
    for (const auto &actor : state.actors) {
        if (actor.get() == self)
            continue;
        const auto *other_enemy = dynamic_cast<const Domain::Entities::Enemy *>(actor.get());
        if (!other_enemy)
            continue;
        if (other_enemy->pos == p)
            return true;
    }
    return false;
}

void EnemyAISystem::action(Domain::Core::GameState &state)
{
    if (state.quit)
        return;

    LOG(DEBUG) << "EnemyAISystem::action()";

    Domain::Entities::Player *player = nullptr;

    for (const auto &actor : state.actors) {
        player = dynamic_cast<Domain::Entities::Player *>(actor.get());
        if (player)
            break;
    }

    if (!player) {
        LOG(DEBUG) << "No player found, skipping enemy AI";
        return;
    }

    for (const auto &actor : state.actors) {
        auto *enemy = dynamic_cast<Domain::Entities::Enemy *>(actor.get());
        if (!enemy)
            continue;

        auto enemy_pos      = enemy->pos;
        auto player_pos     = player->pos;
        bool can_see_player = Domain::Services::LOS::can_see(state.map, enemy_pos, player_pos);

        LOG(DEBUG) << "Enemy at (" << enemy_pos.x << "," << enemy_pos.y << "), state = "
                   << (enemy->state == Domain::Entities::EnemyState::Resting ? "Resting"
                                                                             : "Chasing");

        if (enemy->state == Domain::Entities::EnemyState::Resting && can_see_player) {
            LOG(INFO) << "Enemy spotted player at (" << player_pos.x << "," << player_pos.y
                      << "), switching to Chasing";
            enemy->state = Domain::Entities::EnemyState::Chasing;
        } else if (enemy->state == Domain::Entities::EnemyState::Chasing && !can_see_player) {
            LOG(INFO) << "Enemy lost player at (" << player_pos.x << "," << player_pos.y
                      << "), switching to Resting";
            enemy->state = Domain::Entities::EnemyState::Resting;
        }

        if (enemy->state != Domain::Entities::EnemyState::Chasing)
            continue;
        if (is_adjacent(enemy_pos, player_pos)) {
            LOG(INFO) << "Enemy attacks player!";

            state.intents.push_back({enemy->id, player->id, enemy->stats.atk});
            continue;
        }

        auto dir = Domain::Services::Pathfinding::step_towards(state.map, enemy_pos, player_pos);
        if (!dir.has_value()) {
            LOG(DEBUG) << "No path to player, returning to Resting";
            enemy->state = Domain::Entities::EnemyState::Resting;
            continue;
        }

        Domain::Core::Position next = moved(enemy_pos, *dir);

        if (!state.map.in_bounds(next) || !state.map.is_passable(next) ||
            occupied_by_enemy(state, next, enemy)) {
            LOG(DEBUG) << "Enemy movement blocked, returning to Resting";
            enemy->state = Domain::Entities::EnemyState::Resting;
            continue;
        }

        LOG(DEBUG) << "Enemy moves from (" << enemy_pos.x << "," << enemy_pos.y << ") to ("
                   << next.x << "," << next.y << ")";

        enemy->pos = next;
    }
}
} // namespace Application::Systems
