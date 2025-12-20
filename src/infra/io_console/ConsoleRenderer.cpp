#include "infra/io_console/ConsoleRenderer.hpp"

#include "domain/core/GameState.hpp"
#include "infra/io_console/ConsoleFrameBuilder.hpp"
#include "infra/io_console/ConsoleTerminalAnsi.hpp"
#include "infra/io_console/SymbolSet.hpp"

namespace Infrastructure::IOConsole {

void ConsoleRenderer::draw(const Domain::Core::GameState &state)
{
    const auto &sym = get_symbol_set(symbols_id_);

    const auto frame = ConsoleFrameBuilder::build(state, sym);

    ConsoleTerminalAnsi terminal;
    terminal.draw_full(frame.lines, frame.hud);
}

} // namespace Infrastructure::IOConsole
