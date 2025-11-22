#pragma once
#include "app/loop/IInput.hpp"
#include <optional>

namespace Infrastructure::IOConsole {
class KeyboardInput final : public Application::Loop::IInput {
public:
    std::optional<Application::Loop::InputCommand> poll() override; // stub: nullopt
};
} // namespace Infrastructure::IOConsole
