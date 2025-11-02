#include "domain/services/LOS.hpp"
#include "domain/entities/Map.hpp"

namespace Domain::Services
{
    bool LOS::can_see(const Domain::Entities::Map &, Domain::Core::Position, Domain::Core::Position)
    {
        // TODO: implement Line of Sight (visibility of the blocks)
        return false; 
    }
}
