#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "domain/core/GameState.hpp"
#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/items/Item.hpp"

#include "infra/io_console/ConsoleRenderer.hpp"
#include "test_utils.hpp"

#include "domain/entities/actors/Player.hpp" 

namespace {
using Domain::Core::GameState;
using Domain::Core::Position;
using Domain::Entities::Actor;
using Domain::Entities::Item;
using Domain::Entities::Map;
using Domain::Entities::TileType;

using Infrastructure::IOConsole::ConsoleRenderer;
using Infrastructure::IOConsole::SymbolSetId;

using TestUtils::expect;
using TestUtils::fail_count;

struct TestEnemy final : public Actor {
    void action(GameState &) override {}
};

struct TestItem final : public Item {};

void fill_floor(Map &map)
{
    for (std::size_t y = 0; y < map.height(); ++y) {
        for (std::size_t x = 0; x < map.width(); ++x) {
            auto &t           = map.grid().at(x, y);
            t.type            = TileType::Floor;
            t.blocks_movement = false;
            t.blocks_sight    = false;
        }
    }
}

} // namespace

int main()
{
    GameState state{};
    state.map = Map{10, 5};
    fill_floor(state.map);

    // Place one wall
    {
        auto &t           = state.map.grid().at(0, 0);
        t.type            = TileType::Wall;
        t.blocks_movement = true;
        t.blocks_sight    = true;
    }

    // Player at (1,1)
    {
        auto p = std::make_unique<Domain::Entities::Player>();
        p->pos = Position{1, 1};
        state.actors.push_back(std::move(p));
    }

    // Enemy at (2,1)
    {
        auto e = std::make_unique<TestEnemy>();
        e->pos = Position{2, 1};
        state.actors.push_back(std::move(e));
    }

    // Item at (3,1)
    {
        auto it = std::make_unique<TestItem>();
        it->pos = Position{3, 1};
        state.items.push_back(std::move(it));
    }

    state.turn  = 42;
    state.score = 7;

    ConsoleRenderer renderer{SymbolSetId::Ascii};

    // Capture output
    std::ostringstream capture;
    auto *old = std::cout.rdbuf(capture.rdbuf());

    renderer.draw(state);

    std::cout.rdbuf(old);

    const std::string out = capture.str();

    expect(!out.empty(), "ConsoleRenderer produces output");
    expect(out.find("Turn: 42") != std::string::npos, "HUD contains turn");
    expect(out.find("Score: 7") != std::string::npos, "HUD contains score");

    // ASCII symbols should appear
    expect(out.find("#") != std::string::npos, "Wall (#) present");
    expect(out.find(" ") != std::string::npos, "Floor (.) present");
    expect(out.find("@") != std::string::npos, "Player (@) present");
    expect(out.find("E") != std::string::npos, "Enemy (E) present");
    expect(out.find("*") != std::string::npos, "Item (*) present");

    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
