#include "app/usecases/SaveGameUseCase.hpp"
#include "app/persistence/ISaveGameRepo.hpp"
#include "domain/core/GameState.hpp"

namespace Application::Usecases
{
    bool SaveGameUseCase::save(Application::Persistence::ISaveGameRepo &repo,
                               const Domain::Core::GameState &state,
                               const std::string &path)
    {
        (void)repo;
        (void)state;
        (void)path;
        // TODO: save game

        return false; 
    }
}
