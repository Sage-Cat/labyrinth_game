#pragma once
#include <string>
namespace Domain::Core {
struct GameState;
}
namespace Application::Persistence {
struct ISaveGameRepo;
}

namespace Application::Usecases {
struct LoadGameUseCase {
    static bool load(Application::Persistence::ISaveGameRepo &repo, Domain::Core::GameState &state,
                     const std::string &path);
};
} // namespace Application::Usecases
