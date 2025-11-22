#pragma once
#include <string>
namespace Domain::Core {
struct GameState;
}
namespace Application::Persistence {
struct ISaveGameRepo;
}

namespace Application::Usecases {
struct SaveGameUseCase {
    static bool save(Application::Persistence::ISaveGameRepo &repo,
                     const Domain::Core::GameState &state, const std::string &path);
};
} // namespace Application::Usecases
