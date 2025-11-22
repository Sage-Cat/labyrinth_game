#pragma once
#include "app/loop/IRenderer.hpp"

namespace Infrastructure::IOConsole {
class ConsoleRenderer final : public Application::Loop::IRenderer {
public:
    void draw(const Domain::Core::GameState &) override; // stub: no output
};
} // namespace Infrastructure::IOConsole
