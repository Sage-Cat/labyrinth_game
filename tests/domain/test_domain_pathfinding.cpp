#include <cstdlib>
#include <iostream>

#include "domain/services/Pathfinding.hpp"
#include "domain/entities/Map.hpp"
#include "domain/core/Position.hpp"

int main()
{
    Domain::Entities::Map map{20, 20};
    auto dir = Domain::Services::Pathfinding::step_towards(map, {1, 1}, {5, 5});
    std::cout << "[Pathfinding] step_towards has value: " << (dir.has_value() ? "yes" : "no") << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
