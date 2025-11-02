#pragma once

namespace Domain::Core
{
    struct Stats;
}

namespace Domain::Services
{
    struct DamageCalculator
    {
        static int compute(const Domain::Core::Stats &attacker,
                           const Domain::Core::Stats &defender);
    };
}
