#include "infra/persistence_file/FileSaveGameRepo.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Coin.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/entities/items/Sword.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>

namespace {

constexpr std::string_view PLAYER_TAG        = "PLAYER";
constexpr std::string_view ENEMY_TAG         = "ENEMY";
constexpr std::string_view COIN_TAG          = "COIN";
constexpr std::string_view SWORD_TAG         = "SWORD";
constexpr std::string_view HEALTH_POTION_TAG = "HEALTH_POTION";
constexpr std::string_view KEY_TAG           = "KEY";

bool save_rules(std::ostream &out, const Domain::Rules::GameRules &rules)
{
    out << rules.map_w << '\n';
    out << rules.map_h << '\n';
    out << rules.max_rooms << '\n';
    out << rules.room_min_size << '\n';
    out << rules.room_max_size << '\n';
    out << rules.enemy_count << '\n';
    out << rules.potion_heal_max << '\n';
    out << rules.drop_rates_note << '\n';

    return true;
}
bool load_rules(std::istream &in, Domain::Rules::GameRules &rules)
{
    if (!(in >> rules.map_w))
        return false;
    if (!(in >> rules.map_h))
        return false;
    if (!(in >> rules.max_rooms))
        return false;
    if (!(in >> rules.room_min_size))
        return false;
    if (!(in >> rules.room_max_size))
        return false;
    if (!(in >> rules.enemy_count))
        return false;
    if (!(in >> rules.potion_heal_max))
        return false;

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(in, rules.drop_rates_note);

    return true;
}

bool save_map(std::ostream &out, const Domain::Entities::Map &map)
{
    out << map.height() << ' ' << map.width() << '\n';

    for (size_t y = 0; y < map.height(); y++) {
        for (size_t x = 0; x < map.width(); x++) {
            const auto &tile = map.grid().at(x, y);

            out << static_cast<int>(tile.type) << ' ' << tile.blocks_movement << ' '
                << tile.blocks_sight << '\n';
        }
    }
    return true;
}
bool load_map(std::istream &in, Domain::Entities::Map &map)
{
    std::uint16_t width;
    std::uint16_t height;

    if (!(in >> height >> width))
        return false;

    map = Domain::Entities::Map{width, height};

    for (size_t y = 0; y < map.height(); y++) {
        for (size_t x = 0; x < map.width(); x++) {
            int type;
            bool blocks_movement;
            bool blocks_sight;

            if (!(in >> type >> blocks_movement >> blocks_sight))
                return false;

            auto &tile = map.grid().at(x, y);

            tile.type            = static_cast<Domain::Entities::TileType>(type);
            tile.blocks_movement = blocks_movement;
            tile.blocks_sight    = blocks_sight;
        }
    }
    return true;
}

bool save_actors(std::ostream &out,
                 const std::vector<std::unique_ptr<Domain::Entities::Actor>> &actors)
{
    out << actors.size() << '\n';
    for (const auto &actor : actors) {

        if (auto *player = dynamic_cast<Domain::Entities::Player *>(actor.get())) {
            out << PLAYER_TAG << '\n';
            out << player->has_key << '\n';
        } else if (auto *enemy = dynamic_cast<Domain::Entities::Enemy *>(actor.get())) {
            out << ENEMY_TAG << '\n';
            out << static_cast<int>(enemy->state) << '\n';
        } else {
            return false;
        }

        out << actor->id.value << '\n';
        out << actor->pos.x << ' ' << actor->pos.y << '\n';
        out << actor->stats.hp << ' ' << actor->stats.max_hp << ' ' << actor->stats.atk << ' '
            << actor->stats.def << '\n';
        out << static_cast<int>(actor->glyph.ch) << '\n';
    }

    return true;
}
bool load_actors(std::istream &in, std::vector<std::unique_ptr<Domain::Entities::Actor>> &actors)
{
    actors.clear();

    std::size_t actor_count;
    std::string type;

    if (!(in >> actor_count))
        return false;

    for (std::size_t i = 0; i < actor_count; i++) {
        if (!(in >> type))
            return false;

        std::unique_ptr<Domain::Entities::Actor> actor;

        if (type == PLAYER_TAG) {
            actor        = std::make_unique<Domain::Entities::Player>();
            auto *player = static_cast<Domain::Entities::Player *>(actor.get());
            if (!(in >> player->has_key))
                return false;
        } else if (type == ENEMY_TAG) {
            actor       = std::make_unique<Domain::Entities::Enemy>();
            auto *enemy = static_cast<Domain::Entities::Enemy *>(actor.get());
            int state;
            if (!(in >> state))
                return false;
            enemy->state = static_cast<Domain::Entities::EnemyState>(state);
        } else {
            return false;
        }

        if (!(in >> actor->id.value))
            return false;

        if (!(in >> actor->pos.x >> actor->pos.y))
            return false;

        if (!(in >> actor->stats.hp >> actor->stats.max_hp >> actor->stats.atk >> actor->stats.def))
            return false;

        int glyph_code;
        if (!(in >> glyph_code))
            return false;
        actor->glyph.ch = static_cast<char>(glyph_code);

        actors.push_back(std::move(actor));
    }
    return true;
}

bool save_items(std::ostream &out,
                const std::vector<std::unique_ptr<Domain::Entities::Item>> &items)
{
    out << items.size() << '\n';
    for (const auto &item : items) {
        if (auto *coin = dynamic_cast<Domain::Entities::Coin *>(item.get())) {
            out << COIN_TAG << '\n';
            out << coin->value << '\n';
        } else if (auto *sword = dynamic_cast<Domain::Entities::Sword *>(item.get())) {
            out << SWORD_TAG << '\n';
            out << sword->attack_bonus << '\n';
        } else if (auto *healthpotion =
                       dynamic_cast<Domain::Entities::HealthPotion *>(item.get())) {
            out << HEALTH_POTION_TAG << '\n';
            out << healthpotion->healingValue << '\n';
        } else if (dynamic_cast<Domain::Entities::Key *>(item.get())) {
            out << KEY_TAG << '\n';
        } else {
            return false;
        }

        out << item->id.value << '\n';
        out << item->pos.x << ' ' << item->pos.y << '\n';
        out << static_cast<int>(item->glyph.ch) << '\n';
    }
    return true;
}
bool load_items(std::istream &in, std::vector<std::unique_ptr<Domain::Entities::Item>> &items)
{
    items.clear();

    std::size_t items_count;
    std::string type;

    if (!(in >> items_count))
        return false;

    for (std::size_t i = 0; i < items_count; i++) {
        if (!(in >> type))
            return false;

        std::unique_ptr<Domain::Entities::Item> item;
        if (type == COIN_TAG) {
            item       = std::make_unique<Domain::Entities::Coin>();
            auto *coin = static_cast<Domain::Entities::Coin *>(item.get());
            if (!(in >> coin->value))
                return false;

        } else if (type == SWORD_TAG) {
            item        = std::make_unique<Domain::Entities::Sword>();
            auto *sword = static_cast<Domain::Entities::Sword *>(item.get());
            if (!(in >> sword->attack_bonus))
                return false;

        } else if (type == HEALTH_POTION_TAG) {
            item                = std::make_unique<Domain::Entities::HealthPotion>();
            auto *health_potion = static_cast<Domain::Entities::HealthPotion *>(item.get());
            if (!(in >> health_potion->healingValue))
                return false;

        } else if (type == KEY_TAG) {
            item = std::make_unique<Domain::Entities::Key>();
        } else {
            return false;
        }

        if (!(in >> item->id.value))
            return false;
        if (!(in >> item->pos.x >> item->pos.y))
            return false;

        int glyph_code;
        if (!(in >> glyph_code))
            return false;
        item->glyph.ch = static_cast<char>(glyph_code);

        items.push_back(std::move(item));
    }
    return true;
}
} // namespace

namespace Infrastructure::PersistenceFile {
bool FileSaveGameRepo::save(const Domain::Core::GameState &state, const std::string &path)
{
    std::string tmp_path = path + ".tmp";
    std::ofstream out(tmp_path);

    if (!out)
        return false;

    if (!save_rules(out, state.rules))
        return false;
    if (!save_map(out, state.map))
        return false;
    if (!save_actors(out, state.actors))
        return false;
    if (!save_items(out, state.items))
        return false;

    out << state.defeat << '\n';
    out << state.victory << '\n';
    out << state.score << '\n';
    out << state.turn << '\n';
    out << state.quit << '\n';

    if (!out.good())
        return false;

    out.close();

    std::filesystem::remove(path);
    std::filesystem::rename(tmp_path, path);

    return true;
}

bool FileSaveGameRepo::load(Domain::Core::GameState &state, const std::string &path)
{
    std::ifstream in(path);
    if (!in)
        return false;

    Domain::Core::GameState loaded;

    if (!load_rules(in, loaded.rules)) {
        std::cout << "load_rules failed\n";
        return false;
    }

    if (!load_map(in, loaded.map)) {
        std::cout << "load_map failed\n";
        return false;
    }

    if (!load_actors(in, loaded.actors)) {
        std::cout << "load_actors failed\n";
        return false;
    }
    if (!load_items(in, loaded.items)) {
        std::cout << "load_items failed\n";
        return false;
    }

    if (!(in >> loaded.defeat))
        return false;
    if (!(in >> loaded.victory))
        return false;
    if (!(in >> loaded.score))
        return false;
    if (!(in >> loaded.turn))
        return false;
    if (!(in >> loaded.quit))
        return false;

    state = std::move(loaded);

    return true;
}
} // namespace Infrastructure::PersistenceFile
