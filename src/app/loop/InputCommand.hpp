#pragma once
#include "domain/core/Direction.hpp"
#include <variant>
namespace Application::Loop {
struct CmdMove {
    Domain::Core::Direction dir{};
};
struct CmdWait {};
struct CmdSave {};
struct CmdLoad {};
struct CmdQuit {};
using InputCommand = std::variant<CmdMove, CmdWait, CmdSave, CmdLoad, CmdQuit>;
} // namespace Application::Loop
