
#include "domain/core/IRng.hpp"

struct DummyRng : Domain::Core::IRng
{
    std::uint32_t next_u32() override
    {
        return 0;
    }

    int next_int(int a, int b) override
    {
        (void)a;
        (void)b;
        return a;
    }
};
