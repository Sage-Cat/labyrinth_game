
#include <cassert>
#include <iostream>

#include "domain/core/version.hpp"

static void test_version_is_semver_like() {
    auto v = labyrinth::version();
    // Very light check: "X.Y.Z" should contain two dots.
    int dots = 0;
    for(char c : v) if (c == '.') ++dots;
    assert(dots == 2 && "version() should be semantic-like: X.Y.Z");
}

int main() {
    test_version_is_semver_like();
    std::cout << "[OK] Sanity tests passed\n";
    return 0;
}
