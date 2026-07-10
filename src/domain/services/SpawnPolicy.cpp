#include "domain/services/SpawnPolicy.hpp"

#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Coin.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/entities/items/Sword.hpp"
#include "domain/rules/GameRules.hpp"

#include "infra/log/Logger.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace Domain::Services {
namespace {
using Domain::Core::Position;
using Domain::Entities::Map;

bool same_position(Position a, Position b) noexcept { return a.x == b.x && a.y == b.y; }

bool contains_position(const std::vector<Position> &positions, Position candidate) noexcept
{
    return std::any_of(positions.begin(), positions.end(),
                       [candidate](Position p) { return same_position(p, candidate); });
}

std::vector<Position> collect_floor_positions(const Map &map)
{
    std::vector<Position> floor_positions;
    floor_positions.reserve(static_cast<std::size_t>(map.width()) * map.height());

    for (std::uint16_t y = 0; y < map.height(); ++y) {
        for (std::uint16_t x = 0; x < map.width(); ++x) {
            const Position pos{static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)};
            if (map.is_passable(pos)) {
                floor_positions.push_back(pos);
            }
        }
    }
    return floor_positions;
}

std::optional<Position> pick_unique_floor(const std::vector<Position> &floor_positions,
                                          Domain::Core::IRng &rng, std::vector<Position> &occupied)
{
    if (floor_positions.empty()) {
        return std::nullopt;
    }

    const int last_index = static_cast<int>(floor_positions.size() - 1);
    const int start      = rng.next_int(0, last_index);

    for (std::size_t offset = 0; offset < floor_positions.size(); ++offset) {
        const std::size_t idx = (static_cast<std::size_t>(start) + offset) % floor_positions.size();
        const Position candidate = floor_positions[idx];
        if (!contains_position(occupied, candidate)) {
            occupied.push_back(candidate);
            return candidate;
        }
    }

    return std::nullopt;
}
} // namespace

void SpawnPolicy::place_player(const Domain::Rules::GameRules &, Domain::Core::IRng &rng,
                               Domain::Entities::Map &map, Domain::Entities::Player &player)
{
    LOG(INFO) << "SpawnPolicy::place_player started";
    const auto floor_positions = collect_floor_positions(map);
    std::vector<Position> occupied;

    const auto position = pick_unique_floor(floor_positions, rng, occupied);
    if (!position.has_value()) {
        LOG(ERROR) << "SpawnPolicy::place_player failed: no floor tile available";
        player.pos = Position{0, 0};
    } else {
        player.pos = position.value();
    }

    player.id.value     = 1;
    player.glyph.ch     = '@';
    player.stats.hp     = 20;
    player.stats.max_hp = 20;
    player.stats.atk    = 5;
    player.stats.def    = 1;
    player.has_key      = false;

    LOG(INFO) << "SpawnPolicy::place_player done at (" << static_cast<int>(player.pos.x) << ","
              << static_cast<int>(player.pos.y) << ")";
}

void SpawnPolicy::place_enemies(const Domain::Rules::GameRules &rules, Domain::Core::IRng &rng,
                                Domain::Entities::Map &map,
                                std::vector<Domain::Entities::Enemy> &enemies)
{
    LOG(INFO) << "SpawnPolicy::place_enemies started, requested=" << rules.enemy_count;
    enemies.clear();

    const auto floor_positions = collect_floor_positions(map);
    std::vector<Position> occupied;

    for (std::uint16_t i = 0; i < rules.enemy_count; ++i) {
        const auto position = pick_unique_floor(floor_positions, rng, occupied);
        if (!position.has_value()) {
            LOG(ERROR) << "SpawnPolicy::place_enemies stopped: no free floor tile for enemy " << i;
            break;
        }

        Domain::Entities::Enemy enemy{};
        enemy.id.value     = static_cast<std::uint32_t>(100 + i);
        enemy.pos          = position.value();
        enemy.glyph.ch     = 'E';
        enemy.stats.hp     = 8;
        enemy.stats.max_hp = 8;
        enemy.stats.atk    = 3;
        enemy.stats.def    = 0;
        enemies.push_back(enemy);
    }

    LOG(INFO) << "SpawnPolicy::place_enemies done, placed=" << enemies.size();
}

void SpawnPolicy::place_items(const Domain::Rules::GameRules &rules, Domain::Core::IRng &rng,
                              Domain::Entities::Map &map,
                              std::vector<std::unique_ptr<Domain::Entities::Item>> &items)
{
    LOG(INFO) << "SpawnPolicy::place_items started";
    items.clear();

    const auto floor_positions = collect_floor_positions(map);
    std::vector<Position> occupied;

    const auto key_position = pick_unique_floor(floor_positions, rng, occupied);
    if (!key_position.has_value()) {
        LOG(ERROR) << "SpawnPolicy::place_items failed: no floor tile for key";
        return;
    }

    auto key      = std::make_unique<Domain::Entities::Key>();
    key->id.value = 1000;
    key->pos      = key_position.value();
    key->glyph.ch = 'K';
    items.push_back(std::move(key));

    const std::uint16_t potion_count =
        std::max<std::uint16_t>(1, static_cast<std::uint16_t>(rules.enemy_count / 2));
    for (std::uint16_t i = 0; i < potion_count; ++i) {
        const auto potion_position = pick_unique_floor(floor_positions, rng, occupied);
        if (!potion_position.has_value()) {
            LOG(ERROR) << "SpawnPolicy::place_items stopped: no free floor tile for potion " << i;
            break;
        }

        auto potion          = std::make_unique<Domain::Entities::HealthPotion>();
        potion->id.value     = static_cast<std::uint32_t>(1100 + i);
        potion->pos          = potion_position.value();
        potion->glyph.ch     = '+';
        potion->healingValue = std::max(1, rules.potion_heal_max);
        items.push_back(std::move(potion));
    }

    const auto sword_position = pick_unique_floor(floor_positions, rng, occupied);
    if (sword_position.has_value()) {
        auto sword          = std::make_unique<Domain::Entities::Sword>();
        sword->id.value     = 1200;
        sword->pos          = sword_position.value();
        sword->glyph.ch     = 'S';
        sword->attack_bonus = 5;

        items.push_back(std::move(sword));
    } else {
        LOG(ERROR) << "SpawnPolicy::place_items failed: no floor tile for sword";
    }

    for (std::uint16_t i = 0; i < 2; i++) {
        const auto coin_position = pick_unique_floor(floor_positions, rng, occupied);
        if (!coin_position.has_value()) {
            LOG(ERROR) << "SpawnPolicy::place_items stopped: no free floor tile for coin " << i;
            break;
        }

        auto coin      = std::make_unique<Domain::Entities::Coin>();
        coin->id.value = static_cast<std::uint32_t>(1300 + i);
        coin->pos      = coin_position.value();
        coin->value    = 10;
        coin->glyph.ch = '$';

        items.push_back(std::move(coin));
    }

    LOG(INFO) << "SpawnPolicy::place_items done, placed=" << items.size();
}
} // namespace Domain::Services
