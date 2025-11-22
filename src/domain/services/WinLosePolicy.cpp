#include "domain/services/WinLosePolicy.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Player.hpp"

namespace Domain::Services
{
    void WinLosePolicy::evaluate(Domain::Core::GameState &state)
    {
        for (auto &actor : state.actors)
        {
            auto* player = dynamic_cast<Domain::Entities::Player*>(actor.get());
            if (player && checkPlayerHp(*player))
            {
      state.defeat = true;
       return;
    }
    }
    }
    bool checkPlayerHp( Domain::Entities::Player &player){
    return player.stats.hp <= 0;
    }
}
