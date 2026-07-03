#include "infra/io_console/ConsoleRenderer.hpp"

#include "domain/core/GameState.hpp"
#include "domain/entities/TileType.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Coin.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/entities/items/Sword.hpp"
#include "infra/log/Logger.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32)
#include <io.h>
#endif

namespace Infrastructure::IOConsole {
namespace {
constexpr std::uint8_t kCellWidth = 2;

constexpr const char *kAnsiReset  = "\x1b[0m";
constexpr const char *kAnsiWall   = "\x1b[38;5;240m";
constexpr const char *kAnsiFloor  = "\x1b[38;5;236m";
constexpr const char *kAnsiPlayer = "\x1b[1;32m";
constexpr const char *kAnsiEnemy  = "\x1b[1;31m";
constexpr const char *kAnsiItem   = "\x1b[1;33m";

struct RendererSymbolSet {
    const char *wall{};
    const char *floor{};
    const char *player{};
    const char *enemy{};
    const char *key{};
    const char *potion{};
    const char *sword{};
    const char *coin{};
    const char *fallback{};
    const char *legend{};
    const char *name{};
};

const RendererSymbolSet &get_renderer_symbol_set(SymbolSetId id)
{
    static const RendererSymbolSet kUnicode{
        "██",     "░░",
        "☻ ",     "☠ ",
        "♦ ",     "✚ ",
        "⚔ ",     "🪙 ",
        "· ",     "Legend: ☻ player, ☠ enemy, ♦ key, ✚ potion, ⚔ sword, 🪙 coin, ██ wall, ░░ floor",
        "unicode"};

    static const RendererSymbolSet kAscii{
        "##",   "..",
        "@ ",   "E ",
        "K ",   "+ ",
        "S ",   "$ ",
        "? ",   "Legend: @ player, E enemy, K key, + potion, S sword, $ coin, ## wall, .. floor",
        "ascii"};

    return (id == SymbolSetId::UnicodeSimple) ? kUnicode : kAscii;
}

bool stdout_is_tty()
{
#if defined(__linux__) || defined(__APPLE__)
    return ::isatty(STDOUT_FILENO) != 0;
#elif defined(_WIN32)
    return ::_isatty(_fileno(stdout)) != 0;
#else
    return false;
#endif
}

std::string colorize(const char *code, const std::string &value, bool enabled)
{
    if (!enabled) {
        return value;
    }
    return std::string(code) + value + kAnsiReset;
}

std::string tile_cell(const Domain::Entities::TileType type, const RendererSymbolSet &symbols,
                      bool color_enabled)
{
    if (type == Domain::Entities::TileType::Wall) {
        return colorize(kAnsiWall, symbols.wall, color_enabled);
    }
    return colorize(kAnsiFloor, symbols.floor, color_enabled);
}
} // namespace

void ConsoleRenderer::draw(const Domain::Core::GameState &state)
{
    const auto &symbols = get_renderer_symbol_set(symbols_id_);
    LOG(INFO) << "ConsoleRenderer::draw started, symbols=" << symbols.name;

    const std::uint16_t width  = state.map.width();
    const std::uint16_t height = state.map.height();
    if (width == 0 || height == 0) {
        std::cout << "[empty map]\n";
        LOG(ERROR) << "ConsoleRenderer::draw finished with empty map";
        return;
    }

    const bool color_enabled = stdout_is_tty();
    std::vector<std::vector<std::string>> cells(
        height, std::vector<std::string>(width, std::string(kCellWidth, '#')));

    for (std::uint16_t y = 0; y < height; ++y) {
        for (std::uint16_t x = 0; x < width; ++x) {
            const auto &tile = state.map.grid().at(x, y);
            cells[y][x]      = tile_cell(tile.type, symbols, color_enabled);
        }
    }

    for (const auto &item : state.items) {
        if (!state.map.in_bounds(item->pos)) {
            continue;
        }

        std::string glyph = symbols.fallback;
        if (dynamic_cast<const Domain::Entities::Key *>(item.get()) != nullptr) {
            glyph = symbols.key;
        } else if (dynamic_cast<const Domain::Entities::HealthPotion *>(item.get()) != nullptr) {
            glyph = symbols.potion;
        } else if (dynamic_cast<const Domain::Entities::Sword *>(item.get()) != nullptr) {
            glyph = symbols.sword;
        } else if (dynamic_cast<const Domain::Entities::Coin *>(item.get()) != nullptr) {
            glyph = symbols.coin;
        }
        cells[item->pos.y][item->pos.x] = colorize(kAnsiItem, glyph, color_enabled);
    }

    for (const auto &actor : state.actors) {
        if (!state.map.in_bounds(actor->pos)) {
            continue;
        }

        if (dynamic_cast<const Domain::Entities::Player *>(actor.get()) != nullptr) {
            cells[actor->pos.y][actor->pos.x] =
                colorize(kAnsiPlayer, symbols.player, color_enabled);
        } else if (dynamic_cast<const Domain::Entities::Enemy *>(actor.get()) != nullptr) {
            cells[actor->pos.y][actor->pos.x] = colorize(kAnsiEnemy, symbols.enemy, color_enabled);
        } else {
            cells[actor->pos.y][actor->pos.x] = symbols.fallback;
        }
    }

    if (color_enabled) {
        std::cout << "\x1b[2J\x1b[H";
    }

    const std::string border_line(static_cast<std::size_t>(width) * kCellWidth, '=');
    std::cout << "=== Labyrinth MVP (" << symbols.name << ") ===\n";
    std::cout << "+" << border_line << "+\n";
    for (std::uint16_t y = 0; y < height; ++y) {
        std::cout << "|";
        for (std::uint16_t x = 0; x < width; ++x) {
            std::cout << cells[y][x];
        }
        std::cout << "|\n";
    }
    std::cout << "+" << border_line << "+\n";
    std::cout << "Turn: " << state.turn << " | Map: " << width << "x" << height
              << " | Actors: " << state.actors.size() << " | Items: " << state.items.size() << "\n";
    std::cout << symbols.legend << "\n";
    std::cout << "Controls: w/a/s/d or arrows move, . wait, q quit\n";

    LOG(INFO) << "ConsoleRenderer::draw finished";
}
} // namespace Infrastructure::IOConsole
