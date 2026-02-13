#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <variant>

#include "app/loop/InputCommand.hpp"
#include "domain/core/Direction.hpp"
#include "infra/io_console/KeyboardInput.hpp"

namespace
{
void set_disable_raw_env()
{
#if defined(_WIN32)
    _putenv_s("LABYRINTH_DISABLE_RAW_INPUT", "1");
#else
    setenv("LABYRINTH_DISABLE_RAW_INPUT", "1", 1);
#endif
}

void clear_disable_raw_env()
{
#if defined(_WIN32)
    _putenv_s("LABYRINTH_DISABLE_RAW_INPUT", "");
#else
    unsetenv("LABYRINTH_DISABLE_RAW_INPUT");
#endif
}
} // namespace

int main()
{
    set_disable_raw_env();

    std::istringstream scripted_input("w\n.\nq\n");
    auto *old_buf = std::cin.rdbuf(scripted_input.rdbuf());

    Infrastructure::IOConsole::KeyboardInput in{};

    auto cmd1 = in.poll();
    assert(cmd1.has_value());
    assert(std::holds_alternative<Application::Loop::CmdMove>(*cmd1));
    assert(std::get<Application::Loop::CmdMove>(*cmd1).dir == Domain::Core::Direction::Up);

    auto cmd2 = in.poll();
    assert(cmd2.has_value());
    assert(std::holds_alternative<Application::Loop::CmdWait>(*cmd2));

    auto cmd3 = in.poll();
    assert(cmd3.has_value());
    assert(std::holds_alternative<Application::Loop::CmdQuit>(*cmd3));

    auto cmd4 = in.poll();
    assert(!cmd4.has_value());

    std::cin.rdbuf(old_buf);
    clear_disable_raw_env();
    return EXIT_SUCCESS;
}
