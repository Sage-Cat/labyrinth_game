#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "app/persistence/ISaveGameRepo.hpp"
#include "app/usecases/LoadGameUseCase.hpp"
#include "app/usecases/NewGameUseCase.hpp"
#include "app/usecases/SaveGameUseCase.hpp"
#include "domain/core/GameState.hpp"
#include "domain/core/IRng.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/rules/GameRules.hpp"

namespace
{
class SeededRng final : public Domain::Core::IRng
{
public:
    explicit SeededRng(std::uint32_t seed) : state_(seed) {}

    std::uint32_t next_u32() override
    {
        state_ = state_ * 1664525u + 1013904223u;
        return state_;
    }

    int next_int(int a, int b) override
    {
        if (a > b) {
            const int tmp = a;
            a             = b;
            b             = tmp;
        }

        const std::uint32_t span = static_cast<std::uint32_t>(b - a + 1);
        return a + static_cast<int>(next_u32() % span);
    }

private:
    std::uint32_t state_;
};

struct DummyRepo final : Application::Persistence::ISaveGameRepo
{
    bool save(const Domain::Core::GameState &, const std::string &) override { return false; }
    bool load(Domain::Core::GameState &, const std::string &) override { return false; }
};

std::size_t count_floor_tiles(const Domain::Core::GameState &st)
{
    std::size_t count = 0;
    for (std::uint16_t y = 0; y < st.map.height(); ++y) {
        for (std::uint16_t x = 0; x < st.map.width(); ++x) {
            if (st.map.is_passable({static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)})) {
                ++count;
            }
        }
    }
    return count;
}
} // namespace

int main()
{
    Domain::Rules::GameRules rules{};
    rules.map_w         = 32;
    rules.map_h         = 18;
    rules.enemy_count   = 4;
    rules.potion_heal_max = 9;

    SeededRng rng_1{123456u};
    SeededRng rng_2{123456u};

    Domain::Core::GameState st_1{};
    Domain::Core::GameState st_2{};

    const bool ok_1 = Application::Usecases::NewGameUseCase::execute(st_1, rules, rng_1);
    const bool ok_2 = Application::Usecases::NewGameUseCase::execute(st_2, rules, rng_2);
    assert(ok_1);
    assert(ok_2);

    assert(st_1.rng == &rng_1);
    assert(st_2.rng == &rng_2);
    assert(st_1.rules.map_w == rules.map_w);
    assert(st_1.rules.map_h == rules.map_h);
    assert(st_1.map.width() > 0);
    assert(st_1.map.height() > 0);
    assert(count_floor_tiles(st_1) > 0);

    std::size_t player_count = 0;
    std::size_t enemy_count  = 0;
    std::vector<Domain::Core::Position> actor_positions;
    for (const auto &actor : st_1.actors) {
        assert(st_1.map.in_bounds(actor->pos));
        assert(st_1.map.is_passable(actor->pos));
        actor_positions.push_back(actor->pos);

        if (dynamic_cast<const Domain::Entities::Player *>(actor.get()) != nullptr) {
            ++player_count;
        }
        if (dynamic_cast<const Domain::Entities::Enemy *>(actor.get()) != nullptr) {
            ++enemy_count;
        }
    }
    assert(player_count == 1);
    assert(enemy_count == rules.enemy_count);

    std::size_t key_count = 0;
    for (const auto &item : st_1.items) {
        assert(st_1.map.in_bounds(item->pos));
        assert(st_1.map.is_passable(item->pos));

        if (dynamic_cast<const Domain::Entities::Key *>(item.get()) != nullptr) {
            ++key_count;
        }
    }
    assert(!st_1.items.empty());
    assert(key_count >= 1);

    // Determinism check for seeded RNG: same seed => same spawn layout.
    assert(st_1.actors.size() == st_2.actors.size());
    for (std::size_t i = 0; i < st_1.actors.size(); ++i) {
        assert(st_1.actors[i]->pos.x == st_2.actors[i]->pos.x);
        assert(st_1.actors[i]->pos.y == st_2.actors[i]->pos.y);
    }
    assert(st_1.items.size() == st_2.items.size());
    for (std::size_t i = 0; i < st_1.items.size(); ++i) {
        assert(st_1.items[i]->pos.x == st_2.items[i]->pos.x);
        assert(st_1.items[i]->pos.y == st_2.items[i]->pos.y);
    }

    DummyRepo repo{};
    assert(!Application::Usecases::SaveGameUseCase::save(repo, st_1, "x.sav"));
    assert(!Application::Usecases::LoadGameUseCase::load(repo, st_1, "x.sav"));

    return 0;
}
