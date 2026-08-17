#pragma once
#include "app/loop/InputCommand.hpp"
#include <optional>
namespace Application::Loop {
struct IInput {
    virtual ~IInput()                          = default;
    virtual std::optional<InputCommand> poll() = 0;
};
} // namespace Application::Loop
