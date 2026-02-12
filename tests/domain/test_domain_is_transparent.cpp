#include "../includes/test_utils.hpp"
#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/Tile.hpp"
#include "domain/entities/TileType.hpp"

int main()
{
    Domain::Entities::Map map{10, 10};
    TestUtils::reset_map(map);

    //In-bounds Wall
    TestUtils::expect(map.set_transparent({5, 5}, false), "Setting wall should be succeed");
    TestUtils::expect(!map.is_transparent({5, 5}), "is_transparent Wall");

    //In-bounds Floor
    TestUtils::expect(map.set_transparent({3, 9}, true), "Setting floor should be succeed");
    TestUtils::expect(map.is_transparent({3, 9}) == true, "is_transparent Floor");

    //Out-of-bounds
    TestUtils::expect(!map.is_transparent({10, 10}), "Out-of-bounds");

    TestUtils::reset_map(map);
    return TestUtils::fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}