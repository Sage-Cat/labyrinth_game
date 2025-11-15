#pragma once
#include "domain/rules/GameRules.hpp"

#include "app/usecases/IConfigLoader.hpp"

namespace Infrastructure::Config {
class ConfigLoader : public Application::Usecases::IConfigLoader {
    Domain::Rules::GameRules load_rules() override;
};
} // namespace Infrastructure::Config
