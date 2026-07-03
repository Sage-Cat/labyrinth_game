#include "app/systems/PickupSystem.hpp"
#include <algorithm>

#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Coin.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/entities/items/Sword.hpp"

namespace {
using Domain::Core::GameState;
using Domain::Entities::Player;

Player *find_player(GameState &state)
{
    for (auto &actor : state.actors) {
        if (auto player = dynamic_cast<Player *>(actor.get()))
            return player;
    }
    return nullptr;
}
} // namespace

namespace Application::Systems {

void PickupSystem::process(Domain::Core::GameState &state)
{
    Player *player = find_player(state);
    if (player == nullptr)
        return;

    for (auto it = state.items.begin(); it != state.items.end();) {
        if ((*it)->pos != player->pos) {
            ++it;
            continue;
        }

        if (dynamic_cast<Domain::Entities::Key *>(it->get())) {
            player->has_key = true;
            it              = state.items.erase(it);
        } else if (dynamic_cast<Domain::Entities::HealthPotion *>(it->get())) {
            player->stats.hp = std::min(
                player->stats.hp +
                    dynamic_cast<Domain::Entities::HealthPotion *>(it->get())->healingValue,
                player->stats.max_hp);
            it = state.items.erase(it);
        } else if (dynamic_cast<Domain::Entities::Sword *>(it->get())) {
            player->stats.atk += dynamic_cast<Domain::Entities::Sword *>(it->get())->attack_bonus;
            it = state.items.erase(it);
        } else if (dynamic_cast<Domain::Entities::Coin *>(it->get())) {
            state.score += dynamic_cast<Domain::Entities::Coin *>(it->get())->value;
            it = state.items.erase(it);
        } else {
            ++it;
        }
    }
}
} // namespace Application::Systems
