#pragma once

#include <memory>

#include "IConfigLoader.hpp"

using std::unique_ptr;

namespace Domain::Rules {
struct GameRules;
}

namespace Application::Usecases {
class ConfigLoadUseCase {
public:
    static Domain::Rules::GameRules load_default();

private:
    unique_ptr<IConfigLoader> config_loader;
};
} // namespace Application::Usecases
