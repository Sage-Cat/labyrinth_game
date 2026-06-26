#pragma once
#include <compare>
#include <cstdint>

namespace Domain::Core {
struct EntityId {
    std::uint32_t value{};

    EntityId() = default;
    explicit EntityId(int v) : value(v) {}
    auto operator<=>(const EntityId &) const = default;
};
} // namespace Domain::Core
