#pragma once
#include "domain/entities/actors/Actor.hpp"
#include "domain/entities/actors/EnemyState.hpp"

namespace Domain::Entities {
class Enemy final : public Actor {
public:
    EnemyState state{EnemyState::Resting};
    void action(Domain::Core::GameState &) override;
};
} // namespace Domain::Entities
