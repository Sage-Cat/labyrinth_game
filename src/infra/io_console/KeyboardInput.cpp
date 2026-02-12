#include "infra/io_console/KeyboardInput.hpp"

#include "app/loop/InputCommand.hpp"
#include "domain/core/Direction.hpp"
#include "infra/log/Logger.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <optional>
#include <string>

#if defined(__linux__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <conio.h>
#include <io.h>
#endif

namespace Infrastructure::IOConsole
{
namespace
{
    bool stdin_is_tty()
    {
#if defined(__linux__) || defined(__APPLE__)
        return ::isatty(STDIN_FILENO) != 0;
#elif defined(_WIN32)
        return ::_isatty(_fileno(stdin)) != 0;
#else
        return false;
#endif
    }

#if defined(__linux__) || defined(__APPLE__)
    struct RawModeState
    {
        bool active{false};
        termios saved{};
    };

    RawModeState g_raw_mode{};

    bool enable_raw_mode()
    {
        if (!stdin_is_tty()) {
            return false;
        }
        if (g_raw_mode.active) {
            return true;
        }

        termios current{};
        if (tcgetattr(STDIN_FILENO, &current) != 0) {
            return false;
        }

        g_raw_mode.saved = current;
        termios raw      = current;
        raw.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
        raw.c_iflag &= static_cast<unsigned>(~(IXON | ICRNL));
        raw.c_cc[VMIN]  = 1;
        raw.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0) {
            return false;
        }

        g_raw_mode.active = true;
        return true;
    }

    void disable_raw_mode()
    {
        if (!g_raw_mode.active) {
            return;
        }
        (void)tcsetattr(STDIN_FILENO, TCSANOW, &g_raw_mode.saved);
        g_raw_mode.active = false;
    }

    std::optional<Application::Loop::InputCommand> parse_tty_key()
    {
        unsigned char ch = 0;
        const auto n     = ::read(STDIN_FILENO, &ch, 1);
        if (n <= 0) {
            return Application::Loop::CmdQuit{};
        }

        if (ch == static_cast<unsigned char>(27)) {
            // ANSI arrows: ESC [ A/B/C/D
            unsigned char seq[2]{0, 0};
            if (::read(STDIN_FILENO, &seq[0], 1) <= 0 || ::read(STDIN_FILENO, &seq[1], 1) <= 0) {
                return Application::Loop::CmdWait{};
            }
            if (seq[0] == '[') {
                switch (seq[1]) {
                case 'A':
                    return Application::Loop::CmdMove{Domain::Core::Direction::Up};
                case 'B':
                    return Application::Loop::CmdMove{Domain::Core::Direction::Down};
                case 'C':
                    return Application::Loop::CmdMove{Domain::Core::Direction::Right};
                case 'D':
                    return Application::Loop::CmdMove{Domain::Core::Direction::Left};
                default:
                    return Application::Loop::CmdWait{};
                }
            }
            return Application::Loop::CmdWait{};
        }

        switch (static_cast<char>(std::tolower(ch))) {
        case 'w':
            return Application::Loop::CmdMove{Domain::Core::Direction::Up};
        case 's':
            return Application::Loop::CmdMove{Domain::Core::Direction::Down};
        case 'a':
            return Application::Loop::CmdMove{Domain::Core::Direction::Left};
        case 'd':
            return Application::Loop::CmdMove{Domain::Core::Direction::Right};
        case '.':
            return Application::Loop::CmdWait{};
        case 'q':
            return Application::Loop::CmdQuit{};
        default:
            return Application::Loop::CmdWait{};
        }
    }
#elif defined(_WIN32)
    std::optional<Application::Loop::InputCommand> parse_tty_key()
    {
        const int raw = _getch();

        if (raw == 0 || raw == 224) {
            const int key = _getch();
            switch (key) {
            case 72:
                return Application::Loop::CmdMove{Domain::Core::Direction::Up};
            case 80:
                return Application::Loop::CmdMove{Domain::Core::Direction::Down};
            case 77:
                return Application::Loop::CmdMove{Domain::Core::Direction::Right};
            case 75:
                return Application::Loop::CmdMove{Domain::Core::Direction::Left};
            default:
                return Application::Loop::CmdWait{};
            }
        }

        switch (static_cast<char>(std::tolower(raw))) {
        case 'w':
            return Application::Loop::CmdMove{Domain::Core::Direction::Up};
        case 's':
            return Application::Loop::CmdMove{Domain::Core::Direction::Down};
        case 'a':
            return Application::Loop::CmdMove{Domain::Core::Direction::Left};
        case 'd':
            return Application::Loop::CmdMove{Domain::Core::Direction::Right};
        case '.':
            return Application::Loop::CmdWait{};
        case 'q':
            return Application::Loop::CmdQuit{};
        default:
            return Application::Loop::CmdWait{};
        }
    }
#endif

    std::string trim_copy(std::string line)
    {
        const auto not_space = [](unsigned char ch) { return std::isspace(ch) == 0; };
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), not_space));
        line.erase(std::find_if(line.rbegin(), line.rend(), not_space).base(), line.end());
        return line;
    }

    std::optional<Application::Loop::InputCommand> parse_line_command(std::string input)
    {
        input = trim_copy(std::move(input));
        std::transform(input.begin(), input.end(), input.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

        if (input.empty() || input == ".") {
            return Application::Loop::CmdWait{};
        }
        if (input == "w" || input == "up") {
            return Application::Loop::CmdMove{Domain::Core::Direction::Up};
        }
        if (input == "s" || input == "down") {
            return Application::Loop::CmdMove{Domain::Core::Direction::Down};
        }
        if (input == "a" || input == "left") {
            return Application::Loop::CmdMove{Domain::Core::Direction::Left};
        }
        if (input == "d" || input == "right") {
            return Application::Loop::CmdMove{Domain::Core::Direction::Right};
        }
        if (input == "q" || input == "quit" || input == "exit") {
            return Application::Loop::CmdQuit{};
        }
        if (input == "save" || input == "f5") {
            return Application::Loop::CmdSave{};
        }
        if (input == "load" || input == "f9") {
            return Application::Loop::CmdLoad{};
        }
        return std::nullopt;
    }
} // namespace

KeyboardInput::KeyboardInput()
{
#if defined(__linux__) || defined(__APPLE__)
    raw_mode_enabled_ = enable_raw_mode();
    if (raw_mode_enabled_) {
        LOG(INFO) << "KeyboardInput: raw mode enabled";
    }
#endif
}

KeyboardInput::~KeyboardInput()
{
#if defined(__linux__) || defined(__APPLE__)
    if (raw_mode_enabled_) {
        disable_raw_mode();
        LOG(INFO) << "KeyboardInput: raw mode disabled";
    }
#endif
}

std::optional<Application::Loop::InputCommand> KeyboardInput::poll()
{
    if (!std::cin.good()) {
        LOG(DEBUG) << "KeyboardInput::poll: stdin not ready";
        return std::nullopt;
    }

    if (stdin_is_tty()) {
#if defined(__linux__) || defined(__APPLE__) || defined(_WIN32)
        return parse_tty_key();
#else
        return std::nullopt;
#endif
    }

    std::string line;
    if (!std::getline(std::cin, line)) {
        return std::nullopt;
    }

    auto cmd = parse_line_command(std::move(line));
    if (!cmd.has_value()) {
        return Application::Loop::CmdWait{};
    }
    return cmd;
}
} // namespace Infrastructure::IOConsole
