#pragma once
#include <cstdint>
#include <memory>

namespace Domain::Core {
struct GameState;
}
namespace Application::Loop {
struct IRenderer;
struct IInput;

class GameLoop {
public:
    GameLoop(IInput &input, IRenderer &renderer);
    int run(Domain::Core::GameState &state);

private:
    IInput &input_;
    IRenderer &renderer_;
};
} // namespace Application::Loop
