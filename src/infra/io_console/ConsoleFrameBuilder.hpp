#pragma once

#include "infra/io_console/RenderGrid.hpp"
#include "infra/io_console/SymbolSet.hpp"

#include <string>
#include <vector>

namespace Domain::Core {
struct GameState;
}

namespace Infrastructure::IOConsole {

// Builds renderable grid and HUD from GameState. No terminal IO.
class ConsoleFrameBuilder {
public:
    struct Frame {
        std::vector<std::string> lines;
        std::string hud;
    };

    [[nodiscard]] static Frame build(const Domain::Core::GameState &state, const SymbolSet &sym);

private:
    static RenderGrid build_grid(const Domain::Core::GameState &state, const SymbolSet &sym);
    static std::string build_hud(const Domain::Core::GameState &state, const SymbolSet &sym);
};

} // namespace Infrastructure::IOConsole
