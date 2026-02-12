#include "infra/io_console/ConsoleRenderer.hpp"

#include "infra/log/Logger.hpp"

#include "domain/core/GameState.hpp"
#include "infra/io_console/ConsoleFrameBuilder.hpp"
#include "infra/io_console/ConsoleTerminalAnsi.hpp"
#include "infra/io_console/SymbolSet.hpp"

namespace Infrastructure::IOConsole {

void ConsoleRenderer::draw(const Domain::Core::GameState &state)
{
    LOG(INFO) << "ConsoleRenderer::draw() called";

    const auto &sym = get_symbol_set(symbols_id_);
    LOG(DEBUG) << "Using symbol set ID: " << static_cast<int>(symbols_id_);

    const auto frame = ConsoleFrameBuilder::build(state, sym);
    LOG(DEBUG) << "Frame built with " << frame.lines.size() << " lines and HUD size "
               << frame.hud.size();

    ConsoleTerminalAnsi terminal;
    terminal.draw_full(frame.lines, frame.hud);

    LOG(INFO) << "ConsoleRenderer::draw() finished";
}

} // namespace Infrastructure::IOConsole
