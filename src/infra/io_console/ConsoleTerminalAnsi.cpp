#include "infra/io_console/ConsoleTerminalAnsi.hpp"

#include <iostream>

namespace Infrastructure::IOConsole {

void ConsoleTerminalAnsi::draw_full(const std::vector<std::string> &lines,
                                    const std::string &hud) const
{
    // Clear screen + move cursor to home.
    std::cout << "\x1b[2J\x1b[H";

    for (const auto &line : lines) {
        std::cout << line << '\n';
    }
    std::cout << hud << '\n';
    std::cout.flush();
}

} // namespace Infrastructure::IOConsole
