#include "app/systems/CombatSystem.hpp"
#include "domain/core/GameState.hpp"

#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/services/DamageCalculator.hpp"
#include <algorithm>

namespace Application::Systems {
void CombatSystem::resolve(Domain::Core::GameState &state)
{
    for (const auto &intent : state.intents) {
        auto *attacker = state.find_actor(intent.attacker);
        auto *target   = state.find_actor(intent.target);

        if (!attacker || !target || attacker->stats.hp <= 0 || target->stats.hp <= 0)
            continue;

        const int damage =
            Domain::Services::DamageCalculator::compute(attacker->stats, target->stats);
        target->stats.hp = std::max(0, target->stats.hp - damage);

        if (target->stats.hp == 0) {
            if (dynamic_cast<Domain::Entities::Player *>(target)) {
                state.defeat = true;
            }
        }
    }

    state.actors.erase(std::remove_if(state.actors.begin(), state.actors.end(),
                                      [](const std::unique_ptr<Domain::Entities::Actor> &actor) {
                                          return dynamic_cast<Domain::Entities::Enemy *>(
                                                     actor.get()) != nullptr &&
                                                 actor->stats.hp <= 0;
                                      }),
                       state.actors.end());

    state.intents.clear();
}
} // namespace Application::Systems
