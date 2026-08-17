#include "app/loop/GameLoop.hpp"
#include "app/loop/IInput.hpp"
#include "app/loop/IRenderer.hpp"
#include "domain/core/GameState.hpp"
#include <cstdlib>
#include <optional>

struct DummyInput : Application::Loop::IInput {
    std::optional<Application::Loop::InputCommand> poll() override { return std::nullopt; }
};
struct DummyRenderer : Application::Loop::IRenderer {
    void draw(const Domain::Core::GameState &) override {}
};

int main()
{
    DummyInput in;
    DummyRenderer r;
    Application::Loop::GameLoop loop{in, r};
    Domain::Core::GameState st{};
    int rc = loop.run(st);
    (void)rc; // smoke
    return EXIT_SUCCESS;
}
