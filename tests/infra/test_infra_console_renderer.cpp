#include "domain/core/GameState.hpp"
#include "infra/io_console/ConsoleRenderer.hpp"
#include <cstdlib>

int main()
{
    Infrastructure::IOConsole::ConsoleRenderer r{};
    Domain::Core::GameState state{};
    r.draw(state); // should be a no-op

    // TODO: proper check

    return EXIT_SUCCESS;
}
