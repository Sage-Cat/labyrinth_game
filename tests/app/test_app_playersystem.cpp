#include <iostream>
#include <memory>

#include "app/loop/InputCommand.hpp"
#include "app/systems/PlayerSystem.hpp"

#include "domain/core/AttackIntent.hpp"
#include "domain/core/Direction.hpp"
#include "domain/core/GameState.hpp"

#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"

#include "test_utils.hpp"

using namespace Domain::Core;
using namespace Domain::Entities;
using namespace Application::Systems;
using namespace Application::Loop;
using TestUtils::expect;

int main()
{
    TestUtils::reset_fail_count();

    //Wall collision, test 1
    {
        GameState state;

        state.map = Map(3, 1);

        (void)state.map.set_passable({0, 0}, true);
        (void)state.map.set_passable({1, 0}, true);
        (void)state.map.set_passable({2, 0}, false);

        auto player = std::make_unique<Player>();
        player->id  = EntityId{1};
        player->pos = {1, 0};

        Player *player_ptr = player.get();

        state.actors.push_back(std::move(player));

        PlayerSystem{}.apply(CmdMove{Direction::Right}, state);

        expect(player_ptr->pos == Position{1, 0}, "Player does not move into the wall");
        expect(state.intents.empty(), "No attack intent on wall collision");
    }

    //move on enemy, test 2
    {
        GameState state;
        state.map = Map(3, 1);

        (void)state.map.set_passable({0, 0}, true);
        (void)state.map.set_passable({1, 0}, true);
        (void)state.map.set_passable({2, 0}, true);

        auto player       = std::make_unique<Player>();
        player->id        = EntityId{1};
        player->pos       = {1, 0};
        player->stats.atk = 4;

        auto enemy = std::make_unique<Enemy>();
        enemy->id  = EntityId{2};
        enemy->pos = {2, 0};

        Player *player_ptr = player.get();
        EntityId enemy_id  = enemy->id;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));

        PlayerSystem{}.apply(CmdMove{Direction::Right}, state);

        expect(player_ptr->pos == Position{1, 0}, "Player does not move into the enemy");
        expect(state.intents.size() == 1, "Attack intent created when moving into enemy");

        if (!state.intents.empty()) {
            const auto &intent = state.intents.front();
            expect(intent.attacker == EntityId{1}, "Attacker is player");
            expect(intent.target == enemy_id, "Target is enemy");
            expect(intent.damage == 4, "Damage equals to player's stat");
        }
    }

    std::cout << "Test failed: " << TestUtils::fail_count << "\n";
    return TestUtils::fail_count == 0 ? 0 : 1;
}