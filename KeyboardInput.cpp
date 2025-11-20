#include "infra/io_console/KeyboardInput.hpp"
#include "app/loop/InputCommand.hpp"
#include "domain/core/Direction.hpp"

using namespace Application::Loop;
using namespace Domain::Core;

namespace Infrastructure::IOConsole
{
    std::optional<InputCommand> KeyboardInput::poll()
    {
        return std::nullopt;
    }

    std::optional<InputCommand> KeyboardInput::mapKeyToCommand(int key)
    {
        switch (key)
        {
            case 'w': case 'W': return CmdMove{Direction::Up};
            case 'a': case 'A': return CmdMove{Direction::Left};
            case 's': case 'S': return CmdMove{Direction::Down};
            case 'd': case 'D': return CmdMove{Direction::Right};
            case '.': return CmdWait{};
            case 'q': case 'Q': return CmdQuit{};
            case 0x3F5: return CmdSave{}; // F5
            case 0x3F9: return CmdLoad{}; // F9
            default: return std::nullopt;
        }
    }
}
