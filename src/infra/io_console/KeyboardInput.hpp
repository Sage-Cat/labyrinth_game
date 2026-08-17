#pragma once
#include <optional>
#include "app/loop/IInput.hpp"

namespace Infrastructure::IOConsole
{
    class KeyboardInput final : public Application::Loop::IInput
    {
    public:
        KeyboardInput();
        ~KeyboardInput() override;

        std::optional<Application::Loop::InputCommand> poll() override;

    private:
        bool raw_mode_enabled_{false};
    };
}
