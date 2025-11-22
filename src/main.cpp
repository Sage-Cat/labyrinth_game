#include <cstdint>
#include <string>
#include <string_view>

#include "domain/core/GameState.hpp"
#include "domain/rules/GameRules.hpp"

#include "app/loop/GameLoop.hpp"
#include "app/usecases/ConfigLoadUseCase.hpp"
#include "app/usecases/LoadGameUseCase.hpp"
#include "app/usecases/NewGameUseCase.hpp"

#include "infra/io_console/ConsoleRenderer.hpp"
#include "infra/io_console/KeyboardInput.hpp"
#include "infra/persistence_file/FileSaveGameRepo.hpp"
#include "infra/rng_std/StdRng.hpp"

namespace {
// === Exit codes kept in one place for clarity and tests ===
enum class ExitCode : int {
    Ok                 = 0,
    LoadFailed         = 10,
    NewGameFailed      = 11,
    RuntimeError       = 20,
    UnhandledException = 70
};

struct Cli {
    std::uint32_t seed = 123456789u;
    std::string load_path;
};

// CLI attributes
// Example: ./build/bin/labyrinth --seed=123456789u --path=./data/saves/savefile1.txt
Cli parse_cli(int argc, char **argv)
{
    using std::string_view;
    Cli cli{};
    for (int i = 1; i < argc; ++i) {
        string_view arg{argv[i]};
        if (arg.rfind("--seed=", 0) == 0) {
            const std::string num{arg.substr(7)};
            try {
                unsigned long long v = std::stoull(num);
                cli.seed             = static_cast<std::uint32_t>(v & 0xFFFFFFFFull);
            } catch (...) {
                // ignore malformed seed, keep default
            }
        } else if (arg.rfind("--load=", 0) == 0) {
            cli.load_path = std::string{arg.substr(7)};
        }
    }
    return cli;
}

int app_entry(int argc, char **argv) noexcept
{
    try {
        const Cli cli = parse_cli(argc, argv);

        // Domain setup
        Domain::Core::GameState state{};

        // Adapters (infrastructure)
        Infrastructure::RngStd::StdRng rng{cli.seed};
        Infrastructure::IOConsole::ConsoleRenderer renderer{};
        Infrastructure::IOConsole::KeyboardInput input{};
        Infrastructure::PersistenceFile::FileSaveGameRepo repo{};

        // Bootstrap world (use-cases)

        bool boot_ok = false;
        if (!cli.load_path.empty()) {
            boot_ok = Application::Usecases::LoadGameUseCase::load(repo, state, cli.load_path);
            if (!boot_ok)
                return static_cast<int>(ExitCode::LoadFailed);
        } else {
            boot_ok = Application::Usecases::NewGameUseCase::execute(
                state, Application::Usecases::ConfigLoadUseCase::load_default(), rng);
            if (!boot_ok)
                return static_cast<int>(ExitCode::NewGameFailed);
        }

        // Run loop (application)
        Application::Loop::GameLoop loop{input, renderer};
        const int rc = loop.run(state);
        return rc;
    } catch (const std::bad_alloc &) {
        return static_cast<int>(ExitCode::RuntimeError);
    } catch (const std::exception &) {
        return static_cast<int>(ExitCode::UnhandledException);
    } catch (...) {
        return static_cast<int>(ExitCode::UnhandledException);
    }
}

} // namespace

int main(int argc, char **argv) { return app_entry(argc, argv); }
