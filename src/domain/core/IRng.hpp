#pragma once
#include <cstdint>
#include <utility>
#include <vector>

namespace Domain::Core {
struct IRng {
    virtual ~IRng()                                            = default;
    virtual std::uint32_t next_u32()                           = 0;
    virtual int next_int(int min_inclusive, int max_inclusive) = 0;

    template <class T> void shuffle(std::vector<T> &) {} // header-only no-op (intentionally)
};
} // namespace Domain::Core
