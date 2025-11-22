#include <cstdlib>
#include <iostream>

#include "domain/core/Stats.hpp"
#include "domain/services/DamageCalculator.hpp"

int main()
{
    Domain::Core::Stats a{10, 10, 3, 1};
    Domain::Core::Stats d{10, 10, 2, 2};
    const int dmg = Domain::Services::DamageCalculator::compute(a, d);
    std::cout << "[Damage] compute=" << dmg << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
