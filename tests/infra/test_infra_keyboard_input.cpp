#include "infra/io_console/KeyboardInput.hpp"
#include <cassert>
#include <cstdlib>

int main()
{
    Infrastructure::IOConsole::KeyboardInput in{};
    assert(!in.poll().has_value());

    // TODO: proper check

    return EXIT_SUCCESS;
}
