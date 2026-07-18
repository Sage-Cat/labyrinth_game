#include "infra/rng_std/StdRng.hpp"
#include <cassert>
#include <cstdlib>

int main()
{
    Infrastructure::RngStd::StdRng rng1{123u};
    Infrastructure::RngStd::StdRng rng2{123u};

    for (int i = 0; i < 100; ++i) {
        assert(rng1.next_u32() == rng2.next_u32());
    }

    Infrastructure::RngStd::StdRng rng3{123u};
    Infrastructure::RngStd::StdRng rng4{456u};

    bool different = false;
    for (int i = 0; i < 100; ++i) {
        if (rng3.next_u32() != rng4.next_u32()) {
            different = true;
            break;
        }
    }

    assert(different);
    return EXIT_SUCCESS;
}
