#include <cassert>
#include <cstddef>
#include <cstdint>

#include "domain/entities/Map.hpp"
#include "domain/rules/GameRules.hpp"
#include "domain/services/MapGen.hpp"

#include "dummy_rng.hpp"

int main()
{
    Domain::Rules::GameRules rules{};
    Domain::Entities::Map map{rules.map_w, rules.map_h};
    DummyRng rng{};

    Domain::Services::MapGen::generate(rules, rng, map);

    assert(map.width() == rules.map_w);
    assert(map.height() == rules.map_h);

    std::size_t floor_count = 0;
    for (std::uint16_t y = 0; y < map.height(); ++y) {
        for (std::uint16_t x = 0; x < map.width(); ++x) {
            if (map.is_passable({static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)})) {
                ++floor_count;
            }
        }
    }
    assert(floor_count > 0);

    return 0;
}
