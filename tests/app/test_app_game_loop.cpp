#include "app/loop/GameLoop.hpp"
#include "app/loop/IInput.hpp"
#include "app/loop/IRenderer.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "test_utils.hpp"
#include <cstdlib>
#include <memory>
#include <optional>

struct DummyInput : Application::Loop::IInput {
    std::optional<Application::Loop::InputCommand> poll() override { return std::nullopt; }
};
struct DummyRenderer : Application::Loop::IRenderer {
    void draw(const Domain::Core::GameState &) override { ++draw_count; }
    int draw_count{0};
};

struct WaitInput : Application::Loop::IInput {
    std::optional<Application::Loop::InputCommand> poll() override
    {
        if (used) {
            return std::nullopt;
        }
        used = true;
        return Application::Loop::CmdWait{};
    }
    bool used{false};
};

int main()
{
    using TestUtils::expect;
    TestUtils::reset_fail_count();

    {
        DummyInput in;
        DummyRenderer renderer;
        Application::Loop::GameLoop loop{in, renderer};
        Domain::Core::GameState state{};
        expect(loop.run(state) == 0, "Game loop exits cleanly when input ends");
        expect(renderer.draw_count == 1, "Initial state is rendered");
    }

    {
        WaitInput in;
        DummyRenderer renderer;
        Application::Loop::GameLoop loop{in, renderer};
        Domain::Core::GameState state{};
        state.map = Domain::Entities::Map{3, 3};
        TestUtils::reset_map(state.map);

        auto player       = std::make_unique<Domain::Entities::Player>();
        player->id        = Domain::Core::EntityId{1};
        player->pos       = Domain::Core::Position{1, 1};
        player->stats.hp  = 1;
        player->stats.def = 0;

        auto enemy       = std::make_unique<Domain::Entities::Enemy>();
        enemy->id        = Domain::Core::EntityId{2};
        enemy->pos       = Domain::Core::Position{1, 2};
        enemy->stats.hp  = 10;
        enemy->stats.atk = 2;

        state.actors.push_back(std::move(player));
        state.actors.push_back(std::move(enemy));

        expect(loop.run(state) == 0, "Game loop exits cleanly on defeat");
        expect(state.defeat, "Enemy combat marks the player defeated");
        expect(renderer.draw_count == 2, "Terminal defeat state is rendered once");
    }

    return TestUtils::fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
