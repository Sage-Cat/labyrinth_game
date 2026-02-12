#include "app/loop/GameLoop.hpp"

#include "app/loop/IInput.hpp"
#include "app/loop/IRenderer.hpp"
#include "domain/core/Direction.hpp"
#include "domain/core/GameState.hpp"
#include "domain/core/Position.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/entities/items/Key.hpp"
#include "infra/log/Logger.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <variant>
#include <vector>

namespace Application::Loop
{
namespace
{
    Domain::Entities::Player *find_player(Domain::Core::GameState &state)
    {
        for (auto &actor : state.actors) {
            if (auto *player = dynamic_cast<Domain::Entities::Player *>(actor.get())) {
                return player;
            }
        }
        return nullptr;
    }

    bool enemy_at_position(const Domain::Core::GameState &state, Domain::Core::Position pos)
    {
        for (const auto &actor : state.actors) {
            const auto *enemy = dynamic_cast<const Domain::Entities::Enemy *>(actor.get());
            if (enemy != nullptr && enemy->pos.x == pos.x && enemy->pos.y == pos.y) {
                return true;
            }
        }
        return false;
    }

    Domain::Core::Position moved_position(Domain::Core::Position from, Domain::Core::Direction dir)
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

    void pick_items_on_player_tile(Domain::Core::GameState &state, Domain::Entities::Player &player)
    {
        for (std::size_t i = 0; i < state.items.size();) {
            auto &item = state.items[i];
            if (item->pos.x != player.pos.x || item->pos.y != player.pos.y) {
                ++i;
                continue;
            }

            if (dynamic_cast<Domain::Entities::Key *>(item.get()) != nullptr) {
                player.has_key = true;
                state.score += 100;
                LOG(INFO) << "Player picked up key";
            } else if (auto *potion = dynamic_cast<Domain::Entities::HealthPotion *>(item.get())) {
                const int old_hp = player.stats.hp;
                player.stats.hp =
                    std::min(player.stats.max_hp, player.stats.hp + std::max(1, potion->healingValue));
                LOG(INFO) << "Player used potion, hp " << old_hp << " -> " << player.stats.hp;
            } else {
                LOG(INFO) << "Player picked up unknown item type";
            }

            const auto idx = static_cast<std::vector<std::unique_ptr<Domain::Entities::Item>>::difference_type>(i);
            state.items.erase(state.items.begin() + idx);
        }
    }

    bool try_move_player(Domain::Core::GameState &state, Domain::Entities::Player &player,
                         Domain::Core::Direction dir)
    {
        const auto target = moved_position(player.pos, dir);
        if (target.x == player.pos.x && target.y == player.pos.y) {
            LOG(DEBUG) << "Move ignored: invalid target";
            return false;
        }
        if (!state.map.in_bounds(target)) {
            LOG(DEBUG) << "Move blocked: out of bounds";
            return false;
        }
        if (!state.map.is_passable(target)) {
            LOG(DEBUG) << "Move blocked: wall";
            return false;
        }
        if (enemy_at_position(state, target)) {
            LOG(DEBUG) << "Move blocked: enemy on target tile";
            return false;
        }

        player.pos = target;
        pick_items_on_player_tile(state, player);
        LOG(INFO) << "Player moved to (" << static_cast<int>(player.pos.x) << ","
                  << static_cast<int>(player.pos.y) << ")";
        return true;
    }
} // namespace

GameLoop::GameLoop(IInput &input, IRenderer &renderer)
    : input_(input), renderer_(renderer)
{
}

int GameLoop::run(Domain::Core::GameState &state)
{
    LOG(INFO) << "GameLoop::run started";

    auto *player = find_player(state);
    if (player == nullptr) {
        LOG(ERROR) << "GameLoop::run aborted: player not found";
        return 1;
    }

    bool running = true;
    while (running) {
        renderer_.draw(state);

        const auto command = input_.poll();
        if (!command.has_value()) {
            LOG(DEBUG) << "GameLoop::run no command available, exiting loop";
            break;
        }

        std::visit(
            [&](const auto &cmd) {
                using T = std::decay_t<decltype(cmd)>;
                if constexpr (std::is_same_v<T, CmdMove>) {
                    (void)try_move_player(state, *player, cmd.dir);
                    ++state.turn;
                } else if constexpr (std::is_same_v<T, CmdWait>) {
                    LOG(DEBUG) << "Player waits";
                    ++state.turn;
                } else if constexpr (std::is_same_v<T, CmdSave>) {
                    LOG(INFO) << "Save requested (not yet implemented)";
                    ++state.turn;
                } else if constexpr (std::is_same_v<T, CmdLoad>) {
                    LOG(INFO) << "Load requested (not yet implemented)";
                    ++state.turn;
                } else if constexpr (std::is_same_v<T, CmdQuit>) {
                    LOG(INFO) << "Quit requested";
                    running = false;
                }
            },
            command.value());
    }

    LOG(INFO) << "GameLoop::run finished";
    return 0;
}
} // namespace Application::Loop
