#include "app/usecases/ConfigLoadUseCase.hpp"
#include "domain/rules/GameRules.hpp"
#include "infra/config/ConfigLoader.hpp"

namespace Application::Usecases
{
    Domain::Rules::GameRules ConfigLoadUseCase::load_default()
    {
        return Infrastructure::Config::ConfigLoader::load_default();
    }
}
