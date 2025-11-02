#pragma once
#include <optional>
#include "app/loop/IInput.hpp"

namespace Infrastructure::IOConsole
{
    class KeyboardInput final : public Application::Loop::IInput
    {
    public:
        std::optional<Application::Loop::InputCommand> poll() override; // stub: nullopt
    };
}
