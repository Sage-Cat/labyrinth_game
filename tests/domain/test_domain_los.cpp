#include <cstdlib>
#include <iostream>

#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/services/LOS.hpp"

int main()
{
    Domain::Entities::Map map{20, 20};
    const bool seen = Domain::Services::LOS::can_see(map, Domain::Core::Position{1, 1},
                                                     Domain::Core::Position{5, 5});
    std::cout << "[LOS] can_see result: " << (seen ? "true" : "false") << "\n";

    // TODO: proper check

    return EXIT_SUCCESS;
}
