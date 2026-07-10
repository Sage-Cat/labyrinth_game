#include "app/systems/PlayerSystem.hpp"
#include "app/loop/InputCommand.hpp"
#include "domain/core/AttackIntent.hpp"
#include "domain/core/GameState.hpp"
#include <type_traits>
#include <variant>

namespace Application::Systems {
void PlayerSystem::action(Domain::Core::GameState &)
{
    // TODO: action for player (movement)
}

void PlayerSystem::apply(const Application::Loop::InputCommand &command,
                         Domain::Core::GameState &state)
{
    auto *player = find_player(state);
    if (!player)
        return;

    std::visit(
        [&](const auto &cmd) {
            using T = std::decay_t<decltype(cmd)>;
            if constexpr (std::is_same_v<T, Application::Loop::CmdMove>) {
                try_move_player(state, *player, cmd.dir);
                ++state.turn;
            } else if constexpr (std::is_same_v<T, Application::Loop::CmdWait>) {
                ++state.turn;
            } else if constexpr (std::is_same_v<T, Application::Loop::CmdQuit>) {
                state.quit = true;
            } else if constexpr (std::is_same_v<T, Application::Loop::CmdSave>) {
                ++state.turn;
            } else if constexpr (std::is_same_v<T, Application::Loop::CmdLoad>) {
                ++state.turn;
            }
        },
        command);
}

bool PlayerSystem::try_move_player(Domain::Core::GameState &state, Domain::Entities::Player &player,
                                   Domain::Core::Direction dir)
{
    auto target = moved_position(player.pos, dir);
    if (target.x == player.pos.x && target.y == player.pos.y) {
        return false;
    }
    if (!state.map.in_bounds(target)) {
        return false;
    }
    if (!state.map.is_passable(target)) {
        return false;
    }
    if (auto *enemy = find_enemy_at_position(state, target)) {
        state.intents.push_back(Domain::Core::AttackIntent{player.id, enemy->id, player.stats.atk});
        return false;
    }

    player.pos = target;
    return true;
}

Domain::Entities::Player *PlayerSystem::find_player(Domain::Core::GameState &state)
{
    for (auto &actor : state.actors) {
        if (auto *player = dynamic_cast<Domain::Entities::Player *>(actor.get())) {
            return player;
        }
    }
    return nullptr;
}

Domain::Entities::Enemy *PlayerSystem::find_enemy_at_position(const Domain::Core::GameState &state,
                                                              Domain::Core::Position pos)
{
    for (const auto &actor : state.actors) {
        auto *enemy = dynamic_cast<Domain::Entities::Enemy *>(actor.get());
        if (enemy != nullptr && enemy->pos == pos) {
            return enemy;
        }
    }
    return nullptr;
}

Domain::Core::Position PlayerSystem::moved_position(Domain::Core::Position from,
                                                    Domain::Core::Direction dir)
{
    int x = from.x;
    int y = from.y;
    switch (dir) {
    case Domain::Core::Direction::Up:
        --y;
        break;
    case Domain::Core::Direction::Down:
        ++y;
        break;
    case Domain::Core::Direction::Left:
        --x;
        break;
    case Domain::Core::Direction::Right:
        ++x;
        break;
    }

    if (x < 0 || y < 0 || x > 255 || y > 255) {
        return from;
    }

    return Domain::Core::Position{static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)};
}

} // namespace Application::Systems
