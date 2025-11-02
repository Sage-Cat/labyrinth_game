#pragma once
namespace Domain::Core
{
    struct GameState;
}

namespace Application::Systems
{
    struct WinLoseSystem
    {
        static void evaluate(Domain::Core::GameState &);
    };
}
