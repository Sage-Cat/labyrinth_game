#include <cstdlib>
#include <iostream>
#include "infra/rng_std/StdRng.hpp"

int main()
{
    Infrastructure::RngStd::StdRng rng{123u};
    int v = rng.next_int(1, 6);
    std::cout << "[infra_rng_std] roll=" << v << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
