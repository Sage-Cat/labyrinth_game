#include "app/loop/GameLoop.hpp"
#include "app/loop/IInput.hpp"
#include "app/loop/IRenderer.hpp"
#include "domain/core/GameState.hpp"

namespace Application::Loop
{
    GameLoop::GameLoop(IInput &input, IRenderer &renderer)
        : input_(input), renderer_(renderer) {}

    int GameLoop::run(Domain::Core::GameState &state)
    { 
        // TODO: proper game loop
        (void)state; 
        (void)input_;
        (void)renderer_;
        return 0;
    }
}
