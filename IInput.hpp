#pragma once
#include <optional>
#include "app/loop/InputCommand.hpp"
namespace Application::Loop
{
    struct IInput
    {
        virtual ~IInput() = default;
        virtual std::optional<InputCommand> poll() = 0;
    };
}
