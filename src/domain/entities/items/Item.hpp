#pragma once
#include "domain/core/EntityId.hpp"
#include "domain/core/Position.hpp"

namespace Domain::Entities {
class Item {
public:
    virtual ~Item() = default;

    // Data per diagram
    Domain::Core::EntityId id{};
    Domain::Core::Position pos{};

    // Kind-specific data goes in derived types.
};
} // namespace Domain::Entities
