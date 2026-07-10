#pragma once
#include "domain/entities/items/Item.hpp"

namespace Domain::Entities {
class Sword final : public Item {
public:
    int attack_bonus{};
};
} // namespace Domain::Entities