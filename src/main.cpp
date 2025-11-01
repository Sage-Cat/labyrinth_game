
#include <iostream>
#include "domain/core/version.hpp"

int main(int argc, char** argv) {
    (void)argc; (void)argv; // silence "unused variable warning"
    std::cout << "labyrinth " << labyrinth::version() << "\n";
    std::cout << "Build OK. Game loop not implemented yet.\n";
    return 0;
}
