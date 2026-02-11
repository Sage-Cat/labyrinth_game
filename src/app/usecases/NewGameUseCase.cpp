#include "app/usecases/NewGameUseCase.hpp"
#include "domain/core/GameState.hpp"
#include "domain/core/IRng.hpp"
#include "domain/rules/GameRules.hpp"
#include "infra/log/Logger.hpp"

namespace Application::Usecases {
bool NewGameUseCase::execute(Domain::Core::GameState &, const Domain::Rules::GameRules &,
                             Domain::Core::IRng &)
{
    LOG(INFO) << "Creating new game (not implemented)";
    // TODO: create new game
    return false;
}
} // namespace Application::Usecases
