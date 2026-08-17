#pragma once

namespace Domain::Core {
struct GameState;
struct IRng;
} // namespace Domain::Core
namespace Domain::Rules {
struct GameRules;
}

namespace Application::Usecases {
struct NewGameUseCase {
    // Initialize the state using rules + rng. Returns true on success (stub false).
    static bool execute(Domain::Core::GameState &, const Domain::Rules::GameRules &,
                        Domain::Core::IRng &);
};
} // namespace Application::Usecases
