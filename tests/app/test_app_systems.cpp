#include <cstdlib>

#include "app/systems/CombatSystem.hpp"
#include "app/systems/EnemyAISystem.hpp"
#include "app/systems/PickupSystem.hpp"
#include "app/systems/PlayerSystem.hpp"
#include "app/systems/WinLoseSystem.hpp"
#include "domain/core/GameState.hpp"

int main()
{
    Domain::Core::GameState st{};
    Application::Systems::PlayerSystem{}.action(st);
    Application::Systems::EnemyAISystem{}.action(st);
    Application::Systems::CombatSystem::resolve(st);
    Application::Systems::PickupSystem::process(st);
    Application::Systems::WinLoseSystem::evaluate(st);
    return EXIT_SUCCESS;
}
