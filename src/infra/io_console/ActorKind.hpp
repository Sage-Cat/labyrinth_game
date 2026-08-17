#pragma once

namespace Domain::Entities {
class Actor;
class Player; // forward declare; include Player header in .cpp where used
} // namespace Domain::Entities

namespace Infrastructure::IOConsole {

// Is this actor the player?
[[nodiscard]] bool is_player_actor(const Domain::Entities::Actor &actor) noexcept;

} // namespace Infrastructure::IOConsole
