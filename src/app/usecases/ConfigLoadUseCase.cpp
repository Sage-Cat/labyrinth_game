#include "app/usecases/ConfigLoadUseCase.hpp"
#include "domain/rules/GameRules.hpp"

namespace Application::Usecases {
Domain::Rules::GameRules ConfigLoadUseCase::load_default() { return Domain::Rules::DEFAULT_RULES; }
} // namespace Application::Usecases
