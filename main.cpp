#include <cstdint>
#include <string>
#include <string_view>

#include "domain/core/GameState.hpp"
#include "domain/rules/GameRules.hpp"

#include "app/loop/GameLoop.hpp"
#include "app/usecases/NewGameUseCase.hpp"
#include "app/usecases/LoadGameUseCase.hpp"

#include "infra/rng_std/StdRng.hpp"
#include "infra/io_console/ConsoleRenderer.hpp"
#include "infra/io_console/KeyboardInput.hpp"
#include "infra/persistence_file/FileSaveGameRepo.hpp"

namespace
{
    struct Cli
    {
        std::uint32_t seed = 123456789u;
        std::string load_path;
    };

    Cli parse_cli(int argc, char **argv)
    {
        using std::string_view;
        Cli cli{};
        for (int i = 1; i < argc; ++i)
        {
            string_view arg{argv[i]};
            if (arg.rfind("--seed=", 0) == 0)
            {
                const std::string num{arg.substr(7)};
                try
                {
                    unsigned long long v = std::stoull(num);
                    cli.seed = static_cast<std::uint32_t>(v & 0xFFFFFFFFull);
                }
                catch (...)
                {
                    // ignore malformed seed, keep default
                }
            }
            else if (arg.rfind("--load=", 0) == 0)
            {
                cli.load_path = std::string{arg.substr(7)};
            }
        }
        return cli;
    }
} // namespace

int main(int argc, char **argv)
{
    const Cli cli = parse_cli(argc, argv);

    // Domain setup
    Domain::Rules::GameRules rules{};
    Domain::Core::GameState state{};

    // Adapters (infrastructure)
    Infrastructure::RngStd::StdRng rng{cli.seed};
    Infrastructure::IOConsole::ConsoleRenderer renderer{};
    Infrastructure::IOConsole::KeyboardInput input{};
    Infrastructure::PersistenceFile::FileSaveGameRepo repo{};

    // Bootstrap world (use-cases)
    if (!cli.load_path.empty())
    {
        (void)Application::Usecases::LoadGameUseCase::load(repo, state, cli.load_path);
    }
    else
    {
        (void)Application::Usecases::NewGameUseCase::execute(state, rules, rng);
    }

    // Run loop (application)
    Application::Loop::GameLoop loop{input, renderer};
    const int rc = loop.run(state);
    (void)rc; // TODO: handle exit code

    return 0;
}
