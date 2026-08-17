#include "domain/rules/GameRules.hpp"

namespace Application::Usecases {
struct IConfigLoader {
    virtual Domain::Rules::GameRules load_rules() = 0;
};
} // namespace Application::Usecases