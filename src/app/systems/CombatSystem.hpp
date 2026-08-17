#pragma once
namespace Domain::Core {
struct GameState;
}

namespace Application::Systems {
struct CombatSystem {
    static void resolve(Domain::Core::GameState &);
};
} // namespace Application::Systems
