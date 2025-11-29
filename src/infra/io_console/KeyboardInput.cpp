
#include "infra/io_console/KeyboardInput.hpp"
#include <cctype>
#include <string>

namespace Infrastructure::IOConsole
{
    using Application::Loop::InputCommand;

    
    std::optional<std::string> KeyboardInput::readKey();

    std::optional<InputCommand> KeyboardInput::poll()
    {
        auto keyOpt = readKey();
        if (!keyOpt.has_value())
            return std::nullopt;

        std::string key = *keyOpt;

        // нормалізуємо
        std::string upper;
        upper.reserve(key.size());
        for (char c : key) upper.push_back(std::toupper((unsigned char)c));

        // ---------- arrows ----------
        if (upper == "ARROWUP" || upper == "UP")
            return InputCommand::MOVE_UP;

        if (upper == "ARROWDOWN" || upper == "DOWN")
            return InputCommand::MOVE_DOWN;

        if (upper == "ARROWLEFT" || upper == "LEFT")
            return InputCommand::MOVE_LEFT;

        if (upper == "ARROWRIGHT" || upper == "RIGHT")
            return InputCommand::MOVE_RIGHT;

        // ---------- WASD ----------
        if (upper == "W")
            return InputCommand::MOVE_UP;

        if (upper == "S")
            return InputCommand::MOVE_DOWN;

        if (upper == "A")
            return InputCommand::MOVE_LEFT;

        if (upper == "D")
            return InputCommand::MOVE_RIGHT;

        // ---------- interaction ----------
        if (key == ".")
            return InputCommand::INTERACT;

        // ---------- function keys ----------
        if (upper == "F5")
            return InputCommand::QUICKSAVE;

        if (upper == "F9")
            return InputCommand::QUICKLOAD;

        // ---------- quit ----------
        if (upper == "Q")
            return InputCommand::QUIT;

        return InputCommand::NOOP;
    }
}

