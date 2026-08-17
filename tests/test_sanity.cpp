
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "domain/core/Grid.hpp"
#include "domain/core/Version.hpp"

static void test_version_is_semver_like()
{
    auto v = Domain::Core::version();
    // Very light check: "X.Y.Z" should contain two dots.
    int dots = 0;
    for (char c : v)
        if (c == '.')
            ++dots;
    assert(dots == 2 && "version() should be semantic-like: X.Y.Z");
}

int main()
{
    test_version_is_semver_like();

    Domain::Core::Grid<int> grid{2, 2};
    try {
        (void)grid.at(2, 0);
        std::cerr << "Grid::at accepted an out-of-bounds coordinate\n";
        return EXIT_FAILURE;
    } catch (const std::out_of_range &) {
    }

    std::cout << "[OK] Sanity tests passed\n";
    return EXIT_SUCCESS;
}
