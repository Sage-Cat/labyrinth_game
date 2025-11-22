#pragma once
namespace Domain::Core {
struct GameState;
}

namespace Application::Systems {
struct PickupSystem {
    static void process(Domain::Core::GameState &);
};
} // namespace Application::Systems
