#include "domain/services/WinLosePolicy.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Player.hpp"


namespace  
{  
    bool is_player_defeated(const Domain::Entities::Player &player) noexcept  
    {  
        return player.stats.hp <= 0;  
    }  
}

namespace Domain::Services  
{  
    void WinLosePolicy::evaluate(Domain::Core::GameState &state)  
    {  
        for (auto &actor : state.actors) {  
            if (auto *player = dynamic_cast<Domain::Entities::Player *>(actor.get());  
                player != nullptr && is_player_defeated(*player)) {  
                state.defeat = true;  
                return;  
            }  
        }  
    }  
}  

