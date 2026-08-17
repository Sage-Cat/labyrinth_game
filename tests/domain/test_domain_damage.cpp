#include "domain/core/Stats.hpp"
#include "domain/services/DamageCalculator.hpp"
#include "test_utils.hpp"

int main()
{
    using Domain::Core::Stats;
    using Domain::Services::DamageCalculator;
    using TestUtils::expect;

    TestUtils::reset_fail_count();

    Stats normal_attacker{};
    normal_attacker.atk = 7;

    Stats normal_defender{};
    normal_defender.def = 2;
    expect(DamageCalculator::compute(normal_attacker, normal_defender) == 5,
           "Damage subtracts defender defense");

    Stats strong_defender{};
    strong_defender.def = 20;
    expect(DamageCalculator::compute(normal_attacker, strong_defender) == 1,
           "High defense keeps minimum damage");

    Stats equal_defender{};
    equal_defender.def = 7;
    expect(DamageCalculator::compute(normal_attacker, equal_defender) == 1,
           "Equal attack and defense deal one damage");

    return TestUtils::fail_count == 0 ? 0 : 1;
}
