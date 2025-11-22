#include "domain/services/DamageCalculator.hpp"
#include "domain/core/Stats.hpp"

namespace Domain::Services {
int DamageCalculator::compute(const Domain::Core::Stats &, const Domain::Core::Stats &)
{
    // TODO: calculate damage based on actors stats
    return 0;
}
} // namespace Domain::Services
