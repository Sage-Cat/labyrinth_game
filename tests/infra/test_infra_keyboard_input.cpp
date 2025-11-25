#include <cstdlib>
#include <cassert>
#include <optional>
#include "infra/io_console/KeyboardInput.hpp"

// Уявимо enum для команд
enum class Command {
    Up,
    Down,
    Left,
    Right,
    Action,
    Refresh,
    Special,
    Quit
};

// Функція для тесту мапінгу клавіш
void testKeyboardMapping()
{
    using namespace Infrastructure::IOConsole;

    KeyboardInput in{};

    

    struct TestCase {
        char key;
        Command expected;
    };

    TestCase tests[] = {
        {'w', Command::Up},
        {'W', Command::Up},
        {'s', Command::Down},
        {'S', Command::Down},
        {'a', Command::Left},
        {'A', Command::Left},
        {'d', Command::Right},
        {'D', Command::Right},
        {'.', Command::Action},
        {char(0x74), Command::Refresh}, // F5
        {char(0x78), Command::Special}, // F9
        {'q', Command::Quit},
        {'Q', Command::Quit}
    };

    for (auto &tc : tests)
    {
        in.simulateKeyPress(tc.key); // імітуємо натискання
        auto result = in.poll();

        assert(result.has_value());
        assert(result.value() == tc.expected);
    }

    // Перевірка, що після опрацювання немає додаткових команд
    assert(!in.poll().has_value());
}

int main()
{
    testKeyboardMapping();
    return EXIT_SUCCESS;
}
