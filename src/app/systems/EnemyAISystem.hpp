#pragma once
#include "app/systems/ActorSystem.hpp"

namespace Application::Systems {
class EnemyAISystem final : public ActorSystem {
public:
    void action(Domain::Core::GameState &) override;
};
} // namespace Application::Systems
