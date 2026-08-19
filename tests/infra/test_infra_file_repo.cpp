#include "domain/core/GameState.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Coin.hpp"
#include "domain/rules/GameRules.hpp"
#include "infra/persistence_file/FileSaveGameRepo.hpp"
#include "test_utils.hpp"
#include <cstdlib>

int main()
{
    using namespace Infrastructure::PersistenceFile;
    using namespace Domain::Core;
    using namespace Domain::Entities;
    using TestUtils::expect;

    TestUtils::reset_fail_count();
    {
        GameState state{};
        state.map = Map{3, 3};
        TestUtils::reset_map(state.map);

        state.score   = 100;
        state.turn    = 12;
        state.victory = false;
        state.defeat  = false;
        state.quit    = false;

        state.rules.map_h       = 3;
        state.rules.map_w       = 3;
        state.rules.enemy_count = 1;

        auto player          = std::make_unique<Player>();
        player->id           = EntityId{1};
        player->pos          = {1, 1};
        player->has_key      = true;
        player->stats.hp     = 10;
        player->stats.max_hp = 20;
        player->stats.atk    = 5;
        player->stats.def    = 3;
        player->glyph.ch     = '@';
        state.actors.push_back(std::move(player));

        auto enemy          = std::make_unique<Enemy>();
        enemy->id           = EntityId{2};
        enemy->pos          = {0, 0};
        enemy->state        = EnemyState::Chasing;
        enemy->stats.atk    = 5;
        enemy->stats.def    = 3;
        enemy->stats.hp     = 5;
        enemy->stats.max_hp = 10;
        enemy->glyph.ch     = 'E';
        state.actors.push_back(std::move(enemy));

        auto coin      = std::make_unique<Coin>();
        coin->id       = EntityId{3};
        coin->pos      = {2, 2};
        coin->value    = 10;
        coin->glyph.ch = '$';
        state.items.push_back(std::move(coin));

        FileSaveGameRepo repo{};
        GameState restored{};

        const bool saved = repo.save(state, "test_save.txt");
        expect(saved, "Save Succeeds");

        const bool loaded = repo.load(restored, "test_save.txt");
        expect(loaded, "Load Succeeds");

        if (loaded) {
            expect(restored.rules.map_h == state.rules.map_h, "Map height restored");
            expect(restored.rules.map_w == state.rules.map_w, "Map width restored");
            expect(restored.rules.enemy_count == state.rules.enemy_count,
                   "Map enemy count restored");

            expect(restored.turn == state.turn, "Turn restored");
            expect(restored.score == state.score, "Score restored");
            expect(restored.victory == state.victory, "victory status restored");
            expect(restored.defeat == state.defeat, "defeat status restored");
            expect(restored.quit == state.quit, "Quit status restored");
        }
    }
    return TestUtils::fail_count == 0 ? 0 : 1;
}
