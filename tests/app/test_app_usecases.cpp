#include <cstdlib>
#include <string>

#include "app/usecases/NewGameUseCase.hpp"
#include "app/usecases/SaveGameUseCase.hpp"
#include "app/usecases/LoadGameUseCase.hpp"
#include "app/persistence/ISaveGameRepo.hpp"
#include "domain/core/IRng.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/core/GameState.hpp"

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

int main()
{
    Domain::Core::GameState st{};
    Domain::Rules::GameRules rules{};
    DummyRng rng{};
    (void)Application::Usecases::NewGameUseCase::execute(st, rules, rng);

    DummyRepo repo{};
    (void)Application::Usecases::SaveGameUseCase::save(repo, st, "x.sav");
    (void)Application::Usecases::LoadGameUseCase::load(repo, st, "x.sav");
    return EXIT_SUCCESS;
}
