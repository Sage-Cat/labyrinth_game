#pragma once
#include "domain/core/IRng.hpp"
#include <random>

namespace Infrastructure::RngStd {
class StdRng final : public Domain::Core::IRng {
public:
    explicit StdRng(std::uint32_t seed);
    std::uint32_t next_u32() override;
    int next_int(int a_inclusive, int b_inclusive) override;

private:
    std::mt19937 engine_;
};
} // namespace Infrastructure::RngStd
