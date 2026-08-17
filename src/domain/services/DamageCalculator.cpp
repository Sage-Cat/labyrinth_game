#include "domain/services/DamageCalculator.hpp"
#include "domain/core/Stats.hpp"

#include <algorithm>

namespace Domain::Services {
int DamageCalculator::compute(const Domain::Core::Stats &attacker,
                              const Domain::Core::Stats &defender)
{
    return std::max(1, attacker.atk - defender.def);
}
} // namespace Domain::Services
