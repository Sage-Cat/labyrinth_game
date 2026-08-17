#pragma once
#include "app/loop/InputCommand.hpp"
#include "app/systems/ActorSystem.hpp"
#include "domain/core/Direction.hpp"
#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"

namespace Application::Systems {
class PlayerSystem final : public ActorSystem {
public:
    void action(Domain::Core::GameState &) override;
    void apply(const Application::Loop::InputCommand &command, Domain::Core::GameState &state);

private:
    bool try_move_player(Domain::Core::GameState &state, Domain::Entities::Player &player,
                         Domain::Core::Direction dir);
    Domain::Entities::Player *find_player(Domain::Core::GameState &state);
    Domain::Entities::Enemy *find_enemy_at_position(const Domain::Core::GameState &state,
                                                    Domain::Core::Position pos);
    Domain::Core::Position moved_position(Domain::Core::Position from, Domain::Core::Direction dir);
    void pick_items_on_player_tile(Domain::Core::GameState &state,
                                   Domain::Entities::Player &player);
};
} // namespace Application::Systems
