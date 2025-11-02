#pragma once
#include "domain/core/EntityId.hpp"
#include "domain/core/Position.hpp"
#include "domain/core/Stats.hpp"
#include "domain/core/Glyph.hpp"

namespace Domain::Core
{
    struct GameState;
}

namespace Domain::Entities
{
    class Actor
    {
    public:
        virtual ~Actor() = default;

        // Common composition (matches diagram relations)
        Domain::Core::EntityId id{};
        Domain::Core::Position pos{};
        Domain::Core::Stats stats{};
        Domain::Core::Glyph glyph{};

        // {abstract} action()
        virtual void action(Domain::Core::GameState &) = 0;
    };
}
