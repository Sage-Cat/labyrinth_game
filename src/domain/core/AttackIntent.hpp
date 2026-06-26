#pragma once
#include "domain/core/EntityId.hpp"

namespace Domain::Core {

struct AttackIntent {
    EntityId attacker;
    EntityId target;
    int damage;
};
} // namespace Domain::Core