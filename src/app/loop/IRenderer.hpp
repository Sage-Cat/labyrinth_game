#pragma once
namespace Domain::Core
{
    struct GameState;
}
namespace Application::Loop
{
    struct IRenderer
    {
        virtual ~IRenderer() = default;
        virtual void draw(const Domain::Core::GameState &) = 0;
    };
}
