#include "infra/io_console/ActorKind.hpp"

#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/actors/Player.hpp"

namespace Infrastructure::IOConsole {

bool is_player_actor(const Domain::Entities::Actor &actor) noexcept
{
    // TODO: in future consider having flag or enum instead of relying on RTTI
    return dynamic_cast<const Domain::Entities::Player *>(&actor) != nullptr;
}

} // namespace Infrastructure::IOConsole
