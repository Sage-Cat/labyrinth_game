#include "app/usecases/LoadGameUseCase.hpp"
#include "app/persistence/ISaveGameRepo.hpp"
#include "domain/core/GameState.hpp"
#include "infra/log/Logger.hpp"

namespace Application::Usecases {
bool LoadGameUseCase::load(Application::Persistence::ISaveGameRepo &repo,
                           Domain::Core::GameState &state, const std::string &path)
{
    LOG(INFO) << "LoadGameUseCase::load started, path = " << path;
    (void)repo;
    (void)state;
    (void)path;

    // TODO: load savefile
    LOG(INFO) << "LoadGameUseCase::load finished (stub, returning false)";
    return false;
}
} // namespace Application::Usecases
