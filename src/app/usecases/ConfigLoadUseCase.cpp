#include "app/usecases/ConfigLoadUseCase.hpp"
#include "domain/rules/GameRules.hpp"

namespace Application::Usecases
{
    Domain::Rules::GameRules ConfigLoadUseCase::load_default()
    {
        // TODO: load config
        return Domain::Rules::GameRules{}; 
    }
}
