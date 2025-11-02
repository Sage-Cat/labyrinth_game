#include <cstdlib>
#include <iostream>

#include "domain/services/WinLosePolicy.hpp"
#include "domain/core/GameState.hpp"

int main()
{
    Domain::Core::GameState state{};
    Domain::Services::WinLosePolicy::evaluate(state);
    std::cout << "[WinLose] victory=" << state.victory << " defeat=" << state.defeat << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
