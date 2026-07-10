#pragma once
#include "domain/entities/items/Item.hpp"

namespace Domain::Entities {
class Coin final : public Item {
public:
    int value{};
};
} // namespace Domain::Entities