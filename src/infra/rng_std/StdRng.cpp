#include "infra/rng_std/StdRng.hpp"

namespace Infrastructure::RngStd {
StdRng::StdRng(std::uint32_t seed) : engine_{seed} {}

std::uint32_t StdRng::next_u32()
{
    return static_cast<std::uint32_t>(engine_()); // mt19937 produces 32-bit unsigned
}

int StdRng::next_int(int a, int b)
{
    if (a > b) {
        auto t = a;
        a      = b;
        b      = t;
    }
    std::uniform_int_distribution<int> dist(a, b);
    return dist(engine_);
}
} // namespace Infrastructure::RngStd
