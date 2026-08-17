#include <memory>

#include "app/systems/CombatSystem.hpp"
#include "domain/core/AttackIntent.hpp"
#include "domain/core/GameState.hpp"

#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "test_utils.hpp"

int main()
{
    using namespace Domain::Core;
    using namespace Domain::Entities;
    using namespace Application::Systems;
    using TestUtils::expect;

    TestUtils::reset_fail_count();

    {
        GameState state;

        auto player       = std::make_unique<Player>();
        player->id        = EntityId{1};
        player->stats.hp  = 10;
        player->stats.atk = 7;

        auto enemy       = std::make_unique<Enemy>();
        enemy->id        = EntityId{2};
        enemy->stats.hp  = 10;
        enemy->stats.def = 2;

        const auto player_id = player->id;
        const auto enemy_id  = enemy->id;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));
        state.intents.push_back({player_id, enemy_id, 99});

        CombatSystem::resolve(state);

        auto *enemy_after = state.find_actor(enemy_id);
        expect(enemy_after != nullptr, "Enemy survives normal damage");
        if (enemy_after != nullptr) {
            expect(enemy_after->stats.hp == 5, "Player attack uses attacker and defender stats");
        }
        expect(state.intents.empty(), "Resolved attack intents are cleared");
    }

    {
        GameState state;

        auto player       = std::make_unique<Player>();
        player->id        = EntityId{1};
        player->stats.hp  = 5;
        player->stats.def = 10;

        auto enemy       = std::make_unique<Enemy>();
        enemy->id        = EntityId{2};
        enemy->stats.hp  = 10;
        enemy->stats.atk = 3;

        const auto player_id = player->id;
        const auto enemy_id  = enemy->id;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));
        state.intents.push_back({enemy_id, player_id, 0});

        CombatSystem::resolve(state);

        auto *player_after = state.find_actor(player_id);
        expect(player_after != nullptr, "Player remains after nonlethal damage");
        if (player_after != nullptr) {
            expect(player_after->stats.hp == 4, "Enemy attack deals minimum damage");
        }
        expect(!state.defeat, "Nonlethal damage does not cause defeat");
    }

    {
        GameState state;

        auto player       = std::make_unique<Player>();
        player->id        = EntityId{1};
        player->stats.hp  = 3;
        player->stats.def = 1;

        auto enemy       = std::make_unique<Enemy>();
        enemy->id        = EntityId{2};
        enemy->stats.hp  = 10;
        enemy->stats.atk = 20;

        const auto player_id = player->id;
        const auto enemy_id  = enemy->id;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));
        state.intents.push_back({enemy_id, player_id, 1});

        CombatSystem::resolve(state);

        auto *player_after = state.find_actor(player_id);
        expect(player_after != nullptr, "Player remains in state after defeat");
        if (player_after != nullptr) {
            expect(player_after->stats.hp == 0, "Lethal damage clamps HP to zero");
        }
        expect(state.defeat, "Lethal player damage causes defeat");
    }

    {
        GameState state;

        auto player       = std::make_unique<Player>();
        player->id        = EntityId{1};
        player->stats.hp  = 10;
        player->stats.atk = 3;

        auto enemy      = std::make_unique<Enemy>();
        enemy->id       = EntityId{2};
        enemy->stats.hp = 2;

        const auto player_id = player->id;
        const auto enemy_id  = enemy->id;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));
        state.intents.push_back({player_id, enemy_id, 0});

        CombatSystem::resolve(state);

        expect(state.find_actor(enemy_id) == nullptr, "Dead enemy is removed");
    }

    {
        GameState state;

        auto player       = std::make_unique<Player>();
        player->id        = EntityId{1};
        player->stats.hp  = 10;
        player->stats.atk = 20;

        auto enemy       = std::make_unique<Enemy>();
        enemy->id        = EntityId{2};
        enemy->stats.hp  = 3;
        enemy->stats.atk = 20;

        const auto player_id = player->id;
        const auto enemy_id  = enemy->id;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));
        state.intents.push_back({player_id, enemy_id, 0});
        state.intents.push_back({enemy_id, player_id, 0});

        CombatSystem::resolve(state);

        const auto *player_after = state.find_actor(player_id);
        expect(player_after != nullptr, "Player remains after killing an enemy");
        if (player_after != nullptr) {
            expect(player_after->stats.hp == 10,
                   "A defeated enemy cannot attack later in the turn");
        }
    }

    return TestUtils::fail_count == 0 ? 0 : 1;
}
