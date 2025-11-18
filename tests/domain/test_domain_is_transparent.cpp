#include <cstdlib>
#include <cassert>
#include <iostream>

#include "domain/entities/Map.hpp"
#include "domain/core/Position.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/entities/TileType.hpp"

int main()
{

    Domain::Entities::Map map{10, 10};

    std::cout << "Testing: In-bounds Wall (must be NOT transparent)...\n";
    assert(map.is_transparent({5, 5}) == false);

    Domain::Entities::Tile floorTile{};
    floorTile.type = Domain::Entities::TileType::Floor;
    floorTile.blocks_sight = false;
    floorTile.blocks_movement = false;
    
    map.grid().at(3, 3) = floorTile;

    std::cout << "Testing: In-bounds Floor (must be TRANSPARENT)...\n";
    assert(map.is_transparent({3, 3}) == true);

    std::cout << "Testing: Out-of-bounds (must be NOT transparent)...\n";
    assert(map.is_transparent({10, 10}) == false);
    assert(map.is_transparent({5, 10}) == false);
    assert(map.is_transparent({10, 5}) == false);
    assert(map.is_transparent({200, 200}) == false);

    std::cout << "\n[OK] All test_domain_map_properties passed!\n";
    return EXIT_SUCCESS;
}