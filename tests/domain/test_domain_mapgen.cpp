#include <cstdlib>
#include <iostream>

#include "domain/services/MapGen.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/entities/Map.hpp"

#include "dummy_rng.hpp"

int main()
{
    Domain::Rules::GameRules rules{};
    Domain::Entities::Map map{rules.map_w, rules.map_h};
    DummyRng rng{};
    Domain::Services::MapGen::generate(rules, rng, map);
    std::cout << "[MapGen] generated map: " << map.width() << "x" << map.height() << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
