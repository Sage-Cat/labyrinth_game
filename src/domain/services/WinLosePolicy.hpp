#pragma once

namespace Domain::Core {
struct GameState;
}

namespace Domain::Services {
struct WinLosePolicy {
    static void evaluate(Domain::Core::GameState &state);
};
} // namespace Domain::Services
