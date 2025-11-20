#include <cassert>
#include <iostream>
#include "infra/io_console/KeyboardInput.hpp"

int main()
{
    using namespace Infrastructure::IOConsole;
    using namespace Application::Loop;

    KeyboardInput in{};
    assert(!in.poll().has_value());

    assert(KeyboardInput::mapKeyToCommand('w').has_value());
    assert(KeyboardInput::mapKeyToCommand('a').has_value());
    assert(KeyboardInput::mapKeyToCommand('s').has_value());
    assert(KeyboardInput::mapKeyToCommand('d').has_value());
    assert(KeyboardInput::mapKeyToCommand('.').has_value());
    assert(KeyboardInput::mapKeyToCommand('q').has_value());
    assert(KeyboardInput::mapKeyToCommand(0x3F5).has_value()); // F5
    assert(KeyboardInput::mapKeyToCommand(0x3F9).has_value()); // F9

    std::cout << "All keyboard mapping tests passed\n";
    return EXIT_SUCCESS;
}
// ./test_keyboard
