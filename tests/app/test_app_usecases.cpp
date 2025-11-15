#include <cstdlib>
#include <string>

#include "app/usecases/NewGameUseCase.hpp"
#include "app/usecases/SaveGameUseCase.hpp"
#include "app/usecases/LoadGameUseCase.hpp"
#include "app/persistence/ISaveGameRepo.hpp"
#include "domain/core/IRng.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/core/GameState.hpp"
#include "app/usecases/ConfigLoadUseCase.hpp"

struct DummyRng : Domain::Core::IRng
{
    std::uint32_t next_u32() override { return 0u; }
    int next_int(int a, int) override { return a; }
};

struct DummyRepo : Application::Persistence::ISaveGameRepo
{
    bool save(const Domain::Core::GameState &, const std::string &) override { return false; }
    bool load(Domain::Core::GameState &, const std::string &) override { return false; }
};

bool configload()
{
    auto rules = Application::Usecases::ConfigLoadUseCase::load_default();
    return (rules.map_w == Domain::Rules::DEFAULT_RULES.map_w) &&
           (rules.map_h == Domain::Rules::DEFAULT_RULES.map_h) &&
           (rules.enemy_count == Domain::Rules::DEFAULT_RULES.enemy_count) &&
           (rules.potion_heal_max == Domain::Rules::DEFAULT_RULES.potion_heal_max);
}

int main()
{
    if (!configload())
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
