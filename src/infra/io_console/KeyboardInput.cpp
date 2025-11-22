#include "infra/io_console/KeyboardInput.hpp"
namespace Infrastructure::IOConsole {
std::optional<Application::Loop::InputCommand> KeyboardInput::poll() { return std::nullopt; }
} // namespace Infrastructure::IOConsole
