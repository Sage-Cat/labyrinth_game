#include "infra/io_console/ConsoleFrameBuilder.hpp"

#include "domain/core/GameState.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/items/Item.hpp"

#include "infra/io_console/ActorKind.hpp"
#include "infra/io_console/ConsoleRenderUtils.hpp" 

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>

namespace Infrastructure::IOConsole {

namespace {

using Domain::Core::GameState;
using Domain::Entities::TileType;

[[nodiscard]] bool in_bounds_xy(const GameState &state, std::int32_t x, std::int32_t y)
{
    if (x < 0 || y < 0) {
        return false;
    }
    const auto ux = static_cast<std::size_t>(x);
    const auto uy = static_cast<std::size_t>(y);
    return ux < state.map.width() && uy < state.map.height();
}

[[nodiscard]] std::string tile_to_glyph(const GameState &state, const SymbolSet &sym, std::size_t x,
                                        std::size_t y)
{
    const auto &tile = state.map.grid().at(x, y);

    switch (tile.type) {
    case TileType::Wall:
        return sym.wall;
    case TileType::Floor:
        return sym.floor;
    default:
        return sym.unknown_tile;
    }
}

} // namespace

ConsoleFrameBuilder::Frame ConsoleFrameBuilder::build(const GameState &state, const SymbolSet &sym)
{
    auto grid = build_grid(state, sym);

    Frame frame;
    frame.lines = grid.to_lines();
    frame.hud   = build_hud(state, sym);
    return frame;
}

RenderGrid ConsoleFrameBuilder::build_grid(const GameState &state, const SymbolSet &sym)
{
    const auto w = static_cast<std::size_t>(state.map.width());
    const auto h = static_cast<std::size_t>(state.map.height());

    RenderGrid grid{w, h};

    // Base: tiles.
    for (std::size_t y = 0; y < h; ++y) {
        for (std::size_t x = 0; x < w; ++x) {
            grid.at(x, y) = tile_to_glyph(state, sym, x, y);
        }
    }

    // Items overlay.
    for (const auto &it_ptr : state.items) {
        if (!it_ptr) {
            continue;
        }
        const auto &p = it_ptr->pos;
        if (!in_bounds_xy(state, p.x, p.y)) {
            continue;
        }
        grid.at(static_cast<std::size_t>(p.x), static_cast<std::size_t>(p.y)) = sym.item;
    }

    // Actors overlay (actors override items).
    for (const auto &a_ptr : state.actors) {
        if (!a_ptr) {
            continue;
        }

        const auto &p = a_ptr->pos;
        if (!in_bounds_xy(state, p.x, p.y)) {
            continue;
        }

        std::string g;
        if (auto custom = glyph_to_utf8(a_ptr->glyph); custom.has_value()) {
            g = *custom;
        } else {
            g = is_player_actor(*a_ptr) ? sym.player : sym.enemy;
        }

        grid.at(static_cast<std::size_t>(p.x), static_cast<std::size_t>(p.y)) = std::move(g);
    }

    return grid;
}

std::string ConsoleFrameBuilder::build_hud(const GameState &state, const SymbolSet &sym)
{
    std::ostringstream oss;

    oss << "Turn: " << state.turn << "  "
        << "Score: " << state.score << "  ";

    if (state.victory) {
        oss << "[VICTORY] ";
    }
    if (state.defeat) {
        oss << "[DEFEAT] ";
    }

    // Best-effort player HP from actors[0].
    if (!state.actors.empty() && state.actors[0]) {
        const auto &st = state.actors[0]->stats;
        if constexpr (HasHpAndHpMax<decltype(st)>) {
            oss << sym.hp_label << ": " << st.hp << "/" << st.max_hp;
        } else if constexpr (HasHpOnly<decltype(st)>) {
            oss << sym.hp_label << ": " << st.hp;
        }
    }

    return oss.str();
}

} // namespace Infrastructure::IOConsole
