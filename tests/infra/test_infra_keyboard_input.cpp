#include <cstdlib>
#include <cassert>
#include "infra/io_console/KeyboardInput.hpp"

int main()
{
    Infrastructure::IOConsole::KeyboardInput in{};
    assert(!in.poll().has_value());

    // TODO: proper check

    return EXIT_SUCCESS;
}
