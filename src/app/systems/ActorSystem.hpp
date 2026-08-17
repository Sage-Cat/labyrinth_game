#pragma once
namespace Domain::Core {
struct GameState;
}

namespace Application::Systems {
class ActorSystem {
public:
    virtual ~ActorSystem()                         = default;
    virtual void action(Domain::Core::GameState &) = 0;
};
} // namespace Application::Systems
