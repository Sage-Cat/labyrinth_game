#include "app/loop/GameLoop.hpp"
#include "app/loop/IInput.hpp"
#include "app/loop/IRenderer.hpp"

#include "app/systems/CombatSystem.hpp"
#include "app/systems/EnemyAISystem.hpp"
#include "app/systems/PickupSystem.hpp"
#include "app/systems/PlayerSystem.hpp"

#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "infra/log/Logger.hpp"

namespace Application::Loop {

GameLoop::GameLoop(IInput &input, IRenderer &renderer) : input_(input), renderer_(renderer) {}

int GameLoop::run(Domain::Core::GameState &state)
{
    LOG(INFO) << "GameLoop::run started";

    Application::Systems::PlayerSystem player_system{};
    Application::Systems::EnemyAISystem enemy_ai_system{};
    Application::Systems::CombatSystem combat_system{};
    Application::Systems::PickupSystem pickup_system{};

    bool running = true;

    while (running) {
        renderer_.draw(state);

        const auto command = input_.poll();
        if (!command.has_value()) {
            LOG(DEBUG) << "GameLoop::run no command available, exiting loop";
            break;
        }

        player_system.apply(command.value(), state);

        if (state.quit) {
            LOG(INFO) << "Quit flag set, exiting loop";
            break;
        }

        pickup_system.process(state);
        enemy_ai_system.action(state);
        combat_system.resolve(state);

        for (const auto &actor : state.actors) {
            auto *enemy = dynamic_cast<Domain::Entities::Enemy *>(actor.get());
            if (enemy) {
                LOG(DEBUG) << "Enemy at (" << enemy->pos.x << "," << enemy->pos.y << ") state: "
                           << (enemy->state == Domain::Entities::EnemyState::Resting ? "Resting"
                                                                                     : "Chasing");
            }
        }
    }

    LOG(INFO) << "GameLoop::run finished";
    return 0;
}
} // namespace Application::Loop
