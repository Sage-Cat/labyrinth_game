#pragma once
#include "domain/entities/items/Item.hpp"

namespace Domain::Entities {
class HealthPotion final : public Item {
public:
    int healingValue{}; // +healingValue : int
};
} // namespace Domain::Entities
