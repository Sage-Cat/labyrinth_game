#include "app/usecases/NewGameUseCase.hpp"
#include "domain/core/GameState.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/core/IRng.hpp"

namespace Application::Usecases
{
    bool NewGameUseCase::execute(Domain::Core::GameState &, const Domain::Rules::GameRules &, Domain::Core::IRng &)
    {
        // TODO: create new game
        return false; 
    }
}
