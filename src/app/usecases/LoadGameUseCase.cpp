#include "app/usecases/LoadGameUseCase.hpp"
#include "app/persistence/ISaveGameRepo.hpp"
#include "domain/core/GameState.hpp"

namespace Application::Usecases {
bool LoadGameUseCase::load(Application::Persistence::ISaveGameRepo &repo,
                           Domain::Core::GameState &state, const std::string &path)
{
    (void)repo;
    (void)state;
    (void)path;

    // TODO: load savefile

    return false;
}
} // namespace Application::Usecases
