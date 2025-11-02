#pragma once
#include "domain/core/IRng.hpp"

namespace Domain::Rules
{
    struct GameRules;
}
namespace Domain::Entities
{
    class Map;
}

namespace Domain::Services
{
    struct MapGen
    {
        static void generate(const Domain::Rules::GameRules &rules,
                             Domain::Core::IRng &rng,
                             Domain::Entities::Map &map);
    };
}
