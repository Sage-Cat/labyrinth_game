#include <cstdlib>
#include <iostream>
#include <string>

#include "app/persistence/ISaveGameRepo.hpp"
#include "app/usecases/ConfigLoadUseCase.hpp"
#include "app/usecases/LoadGameUseCase.hpp"
#include "app/usecases/NewGameUseCase.hpp"
#include "app/usecases/SaveGameUseCase.hpp"
#include "domain/core/GameState.hpp"
#include "domain/core/IRng.hpp"
#include "domain/rules/GameRules.hpp"

struct DummyRng : Domain::Core::IRng {
    std::uint32_t next_u32() override { return 0u; }
    int next_int(int a, int) override { return a; }
};

struct DummyRepo : Application::Persistence::ISaveGameRepo {
    bool save(const Domain::Core::GameState &, const std::string &) override { return false; }
    bool load(Domain::Core::GameState &, const std::string &) override { return false; }
};

[[nodiscard]] bool test_config_load_returns_default_rules()
{
    const auto expected = Domain::Rules::DEFAULT_RULES;
    const auto rules    = Application::Usecases::ConfigLoadUseCase::load_default();

    bool ok = true;

    const auto check_equal = [&](auto actual, auto exp, std::string_view name) {
        if (actual != exp) {
            std::cerr << "ConfigLoadUseCase::load_default(): '" << name
                      << "' mismatch: actual=" << actual << ", expected=" << exp << '\n';
            ok = false;
        }
    };

    check_equal(rules.map_w, expected.map_w, "map_w");
    check_equal(rules.map_h, expected.map_h, "map_h");
    check_equal(rules.enemy_count, expected.enemy_count, "enemy_count");
    check_equal(rules.potion_heal_max, expected.potion_heal_max, "potion_heal_max");

    return ok;
}

[[nodiscard]] bool test_new_game_executes()
{
    Domain::Core::GameState st{};
    Domain::Rules::GameRules rules{};
    DummyRng rng{};

    Application::Usecases::NewGameUseCase::execute(st, rules, rng);
    // TODO: add real assertions on `st` and `rules`
    return true;
}

[[nodiscard]] bool test_save_and_load_do_not_crash()
{
    Domain::Core::GameState st{};
    DummyRepo repo{};

    Application::Usecases::SaveGameUseCase::save(repo, st, "x.sav");
    Application::Usecases::LoadGameUseCase::load(repo, st, "x.sav");

    // TODO: add expectations when DummyRepo is improved
    return true;
}

int main()
{
    bool alright = true;
    alright &= test_config_load_returns_default_rules();
    alright &= test_new_game_executes();
    alright &= test_save_and_load_do_not_crash();

    return alright ? EXIT_SUCCESS : EXIT_FAILURE;
}
