#pragma once
#include "domain/entities/actors/Actor.hpp"

namespace Domain::Entities
{
    class Player final : public Actor
    {
    public:
        bool has_key{false};
        void action(Domain::Core::GameState &) override;
    };
}
