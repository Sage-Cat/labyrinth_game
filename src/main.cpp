#include <cstdint>
#include <cstdlib>
#include <exception>
#include <new>
#include <optional>
#include <random>
#include <string>
#include <string_view>

#include "domain/core/GameState.hpp"
#include "domain/rules/GameRules.hpp"

#include "app/loop/GameLoop.hpp"
#include "app/usecases/LoadGameUseCase.hpp"
#include "app/usecases/NewGameUseCase.hpp"

#include "infra/io_console/ConsoleRenderer.hpp"
#include "infra/io_console/KeyboardInput.hpp"
#include "infra/log/Logger.hpp"
#include "infra/persistence_file/FileSaveGameRepo.hpp"
#include "infra/rng_std/StdRng.hpp"

namespace {
enum class ExitCode : int {
    Ok                 = 0,
    LoadFailed         = 10,
    NewGameFailed      = 11,
    RuntimeError       = 20,
    UnhandledException = 70
};

struct Cli {
    std::optional<std::uint32_t> seed;
    std::string load_path;
    Infrastructure::IOConsole::SymbolSetId symbols{Infrastructure::IOConsole::SymbolSetId::Ascii};
};

Cli parse_cli(int argc, char **argv)
{
    using std::string_view;
    Cli cli{};
    for (int i = 1; i < argc; ++i) {
        string_view arg{argv[i]};
        if (arg.rfind("--seed=", 0) == 0) {
            const std::string num{arg.substr(7)};
            try {
                const unsigned long long v = std::stoull(num);
                cli.seed                   = static_cast<std::uint32_t>(v & 0xFFFFFFFFull);
            } catch (...) {
                // Ignore malformed seed and keep default.
            }
        } else if (arg.rfind("--load=", 0) == 0) {
            cli.load_path = std::string{arg.substr(7)};
        } else if (arg.rfind("--symbols=", 0) == 0) {
            const auto value = arg.substr(10);
            if (value == "ascii") {
                cli.symbols = Infrastructure::IOConsole::SymbolSetId::Ascii;
            } else if (value == "unicode") {
                cli.symbols = Infrastructure::IOConsole::SymbolSetId::UnicodeSimple;
            }
        } else if (arg == "--ascii") {
            cli.symbols = Infrastructure::IOConsole::SymbolSetId::Ascii;
        } else if (arg == "--unicode") {
            cli.symbols = Infrastructure::IOConsole::SymbolSetId::UnicodeSimple;
        }
    }

    if (const char *env_symbols = std::getenv("LABYRINTH_SYMBOLS")) {
        const std::string_view env_value{env_symbols};
        if (env_value == "ascii") {
            cli.symbols = Infrastructure::IOConsole::SymbolSetId::Ascii;
        } else if (env_value == "unicode") {
            cli.symbols = Infrastructure::IOConsole::SymbolSetId::UnicodeSimple;
        }
    }

    return cli;
}

int app_entry(int argc, char **argv) noexcept
{
    Infrastructure::Log::Logger::instance().init_file("labyrinth.log");
    LOG(INFO) << "Labyrinth startup";

    try {
        const Cli cli = parse_cli(argc, argv);

        const std::uint32_t seed = cli.seed.has_value() ? *cli.seed : std::random_device{}();
        LOG(INFO) << "Using RNG seed " << seed;

        Domain::Rules::GameRules rules{};
        Domain::Core::GameState state{};

        Infrastructure::RngStd::StdRng rng{seed};
        Infrastructure::IOConsole::ConsoleRenderer renderer{cli.symbols};
        Infrastructure::IOConsole::KeyboardInput input{};
        Infrastructure::PersistenceFile::FileSaveGameRepo repo{};

        bool boot_ok = false;
        if (!cli.load_path.empty()) {
            boot_ok = Application::Usecases::LoadGameUseCase::load(repo, state, cli.load_path);
            if (!boot_ok) {
                LOG(ERROR) << "Load game failed for path: " << cli.load_path;
                return static_cast<int>(ExitCode::LoadFailed);
            }
        } else {
            boot_ok = Application::Usecases::NewGameUseCase::execute(state, rules, rng);
            if (!boot_ok) {
                LOG(ERROR) << "New game creation failed";
                return static_cast<int>(ExitCode::NewGameFailed);
            }
        }

        Application::Loop::GameLoop loop{input, renderer};
        const int rc = loop.run(state);

        LOG(INFO) << "Game loop exited with code " << rc;
        return rc;
    } catch (const std::bad_alloc &) {
        LOG(ERROR) << "Runtime error: out of memory";
        return static_cast<int>(ExitCode::RuntimeError);
    } catch (const std::exception &ex) {
        LOG(ERROR) << "Unhandled exception: " << ex.what();
        return static_cast<int>(ExitCode::UnhandledException);
    } catch (...) {
        LOG(ERROR) << "Unhandled non-standard exception";
        return static_cast<int>(ExitCode::UnhandledException);
    }
}
} // namespace

int main(int argc, char **argv) { return app_entry(argc, argv); }
