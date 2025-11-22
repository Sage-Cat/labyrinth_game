#include <cstdlib>
#include <iostream>

#include "domain/services/WinLosePolicy.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Player.hpp"

bool testHpLessThanZero()
{
Domain::Core::GameState state{};
auto player = std::make_unique<Domain::Entities::Player>();
player->stats.hp = -5;
state.actors.push_back(std::move(player));
Domain::Services::WinLosePolicy::evaluate(state);
return state.defeat == true;
std::cout << "[WinLose] defeat=" << state.defeat << "\n";
}

bool testHpEquelZero()
{
Domain::Core::GameState state{};
auto player = std::make_unique<Domain::Entities::Player>();
player->stats.hp = 0;
state.actors.push_back(std::move(player));
Domain::Services::WinLosePolicy::evaluate(state);
return state.defeat == true;
std::cout << "[WinLose] defeat=" << state.defeat << "\n";
}

bool testHpGreaterThanZero()
{
Domain::Core::GameState state{};
auto player = std::make_unique<Domain::Entities::Player>();
player->stats.hp = 5;
state.actors.push_back(std::move(player));
Domain::Services::WinLosePolicy::evaluate(state);
return state.defeat == false;

}
int main()
{
   if(testHpLessThanZero() && testHpEquelZero() && testHpGreaterThanZero())
    return EXIT_SUCCESS;
    else 
    return EXIT_FAILURE;

}
