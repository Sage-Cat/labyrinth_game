#pragma once
#include "domain/rules/GameRules.hpp"

namespace Infrastructure::Config
{
    struct ConfigLoader
    {
        static Domain::Rules::GameRules load_default() { return Domain::Rules::GameRules{}; }
    };
}
