#pragma once

#include <string>
#include <vector>

namespace Infrastructure::IOConsole {

// Minimal ANSI backend: full redraw.
class ConsoleTerminalAnsi {
public:
    void draw_full(const std::vector<std::string> &lines, const std::string &hud) const;
};

} // namespace Infrastructure::IOConsole
