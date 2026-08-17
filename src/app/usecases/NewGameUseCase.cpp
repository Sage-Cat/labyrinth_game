#include "app/usecases/NewGameUseCase.hpp"

#include "domain/core/GameState.hpp"
#include "domain/core/Position.hpp"
#include "domain/core/IRng.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/services/MapGen.hpp"
#include "domain/services/SpawnPolicy.hpp"

#include "infra/log/Logger.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace Application::Usecases
{
namespace
{
    using Domain::Core::Position;

    bool same_position(Position a, Position b) noexcept
    {
        return a.x == b.x && a.y == b.y;
    }

    bool contains_position(const std::vector<Position> &positions, Position candidate) noexcept
    {
        for (const auto &pos : positions) {
            if (same_position(pos, candidate)) {
                return true;
            }
        }
        return false;
    }

    bool has_floor_tiles(const Domain::Entities::Map &map)
    {
        for (std::uint16_t y = 0; y < map.height(); ++y) {
            for (std::uint16_t x = 0; x < map.width(); ++x) {
                const Position pos{static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)};
                if (map.is_passable(pos)) {
                    return true;
                }
            }
        }
        return false;
    }

    std::optional<Position> find_first_free_floor(const Domain::Entities::Map &map,
                                                  const std::vector<Position> &occupied)
    {
        for (std::uint16_t y = 0; y < map.height(); ++y) {
            for (std::uint16_t x = 0; x < map.width(); ++x) {
                const Position pos{static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)};
                if (!map.is_passable(pos)) {
                    continue;
                }
                if (!contains_position(occupied, pos)) {
                    return pos;
                }
            }
        }
        return std::nullopt;
    }

    void normalize_position(Position &position, const Domain::Entities::Map &map,
                            std::vector<Position> &occupied)
    {
        const bool valid = map.in_bounds(position) && map.is_passable(position) &&
                           !contains_position(occupied, position);
        if (valid) {
            occupied.push_back(position);
            return;
        }

        const auto replacement = find_first_free_floor(map, occupied);
        if (replacement.has_value()) {
            position = replacement.value();
            occupied.push_back(position);
        }
    }
} // namespace

bool NewGameUseCase::execute(Domain::Core::GameState &state, const Domain::Rules::GameRules &rules,
                             Domain::Core::IRng &rng)
{
    LOG(INFO) << "NewGameUseCase::execute started";

    state = Domain::Core::GameState{};
    state.rules = rules;
    state.rng   = &rng;

    Domain::Services::MapGen::generate(state.rules, rng, state.map);
    if (state.map.width() == 0 || state.map.height() == 0 || !has_floor_tiles(state.map)) {
        LOG(ERROR) << "NewGameUseCase::execute failed: generated map is empty or has no floor";
        return false;
    }

    Domain::Entities::Player player{};
    Domain::Services::SpawnPolicy::place_player(state.rules, rng, state.map, player);

    std::vector<Domain::Entities::Enemy> enemies;
    Domain::Services::SpawnPolicy::place_enemies(state.rules, rng, state.map, enemies);
    Domain::Services::SpawnPolicy::place_items(state.rules, rng, state.map, state.items);

    state.actors.clear();
    state.actors.reserve(static_cast<std::size_t>(1 + enemies.size()));
    state.actors.push_back(std::make_unique<Domain::Entities::Player>(std::move(player)));
    for (const auto &enemy : enemies) {
        state.actors.push_back(std::make_unique<Domain::Entities::Enemy>(enemy));
    }

    std::vector<Position> occupied_positions;
    for (auto &actor : state.actors) {
        normalize_position(actor->pos, state.map, occupied_positions);
    }
    for (auto &item : state.items) {
        normalize_position(item->pos, state.map, occupied_positions);
    }

    std::size_t player_count = 0;
    std::size_t enemy_count  = 0;
    for (const auto &actor : state.actors) {
        if (dynamic_cast<const Domain::Entities::Player *>(actor.get()) != nullptr) {
            ++player_count;
        } else if (dynamic_cast<const Domain::Entities::Enemy *>(actor.get()) != nullptr) {
            ++enemy_count;
        }
    }

    std::size_t key_count = 0;
    for (const auto &item : state.items) {
        if (dynamic_cast<const Domain::Entities::Key *>(item.get()) != nullptr) {
            ++key_count;
        }
    }

    if (player_count != 1) {
        LOG(ERROR) << "NewGameUseCase::execute failed: expected exactly one player, got "
                   << player_count;
        return false;
    }
    if (enemy_count == 0 && state.rules.enemy_count > 0) {
        LOG(ERROR) << "NewGameUseCase::execute failed: expected enemies, got zero";
        return false;
    }
    if (state.items.empty() || key_count == 0) {
        LOG(ERROR) << "NewGameUseCase::execute failed: item placement incomplete";
        return false;
    }

    LOG(INFO) << "NewGameUseCase::execute done, map=" << state.map.width() << "x"
              << state.map.height() << ", actors=" << state.actors.size()
              << ", items=" << state.items.size();
    return true;
}
} // namespace Application::Usecases
