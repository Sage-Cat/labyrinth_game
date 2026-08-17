#include "domain/core/Position.hpp"
#include "domain/entities/Map.hpp"
#include "domain/services/Pathfinding.hpp"
#include "test_utils.hpp"
#include <cstdlib>
#include <iostream>

int main()
{
    using Domain::Core::Direction;
    using Domain::Core::Position;
    using Domain::Entities::Map;
    using Domain::Services::Pathfinding;

    TestUtils::reset_fail_count();

    //test1
    {
        Map map{5, 5};
        TestUtils::expect(map.set_passable({0, 2}, true), "[Test1] set_passable {0,2}");
        TestUtils::expect(map.set_passable({4, 2}, true), "[Test1] set_passable {4,2}");

        TestUtils::expect(map.set_passable({1, 2}, true), "[Test1] set_passable {1,2}");
        TestUtils::expect(map.set_passable({2, 2}, true), "[Test1] set_passable {2,2}");
        TestUtils::expect(map.set_passable({3, 2}, true), "[Test1] set_passable {3,2}");

        auto dir = Pathfinding::step_towards(map, {0, 2}, {4, 2});
        TestUtils::expect(dir.has_value(), "[Test1] step_towards should return a direction");
        if (dir) {
            TestUtils::expect(*dir == Direction::Right, "[Test1] expected Right direction");
        }
    }

    //test2
    //detour1
    {
        Map map{5, 5};
        TestUtils::expect(map.set_passable({1, 2}, true), "[Test2.1] set_passable {1,2}");
        TestUtils::expect(map.set_passable({3, 2}, true), "[Test2.1] set_passable {3,2}");

        TestUtils::expect(map.set_passable({1, 3}, true), "[Test2.1] set_passable {1,3}");
        TestUtils::expect(map.set_passable({2, 3}, true), "[Test2.1] set_passable {2,3}");
        TestUtils::expect(map.set_passable({3, 3}, true), "[Test2.1] set_passable {3,3}");

        auto dir = Pathfinding::step_towards(map, {1, 2}, {3, 2});
        TestUtils::expect(dir.has_value(), "[Test2.1] step_towards should return a direction");

        if (dir) {
            TestUtils::expect(*dir == Direction::Down, "[Test2.1] expected Down direction");
        }
    }
    //detour2
    {
        Map map{5, 5};
        TestUtils::expect(map.set_passable({1, 2}, true), "[Test2.2] set_passable {1,2}");
        TestUtils::expect(map.set_passable({3, 2}, true), "[Test2.2] set_passable {3,2}");

        TestUtils::expect(map.set_passable({1, 0}, true), "[Test2.2] set_passable {1,0}");
        TestUtils::expect(map.set_passable({2, 0}, true), "[Test2.2] set_passable {2,0}");
        TestUtils::expect(map.set_passable({3, 0}, true), "[Test2.2] set_passable {3,0}");
        TestUtils::expect(map.set_passable({1, 1}, true), "[Test2.2] set_passable {1,1}");
        TestUtils::expect(map.set_passable({3, 1}, true), "[Test2.2] set_passable {3,1}");
        TestUtils::expect(map.set_passable({1, 3}, true), "[Test2.2] set_passable {1,3}");
        TestUtils::expect(map.set_passable({3, 3}, true), "[Test2.2] set_passable {3,3}");
        TestUtils::expect(map.set_passable({1, 4}, true), "[Test2.2] set_passable {1,4}");
        TestUtils::expect(map.set_passable({2, 4}, true), "[Test2.2] set_passable {2,4}");
        TestUtils::expect(map.set_passable({3, 4}, true), "[Test2.2] set_passable {3,4}");

        auto dir = Pathfinding::step_towards(map, {1, 2}, {3, 2});
        TestUtils::expect(dir.has_value(), "[Test2.2] step_towards should return a direction");

        if (dir) {
            TestUtils::expect(*dir == Direction::Up || *dir == Direction::Down,
                              "[Test2.2] expected Up or Down direction");
        }
    }
    //detour3
    {
        Map map{5, 5};
        TestUtils::expect(map.set_passable({0, 2}, true), "[Test2.3] set_passable {0,2}");
        TestUtils::expect(map.set_passable({4, 2}, true), "[Test2.3] set_passable {4,2}");

        TestUtils::expect(map.set_passable({0, 0}, true), "[Test2.3] set_passable {0,0}");
        TestUtils::expect(map.set_passable({1, 0}, true), "[Test2.3] set_passable {1,0}");
        TestUtils::expect(map.set_passable({2, 0}, true), "[Test2.3] set_passable {2,0}");
        TestUtils::expect(map.set_passable({3, 0}, true), "[Test2.3] set_passable {3,0}");
        TestUtils::expect(map.set_passable({4, 0}, true), "[Test2.3] set_passable {4,0}");
        TestUtils::expect(map.set_passable({0, 1}, true), "[Test2.3] set_passable {0,1}");
        TestUtils::expect(map.set_passable({4, 1}, true), "[Test2.3] set_passable {4,1}");
        TestUtils::expect(map.set_passable({1, 2}, true), "[Test2.3] set_passable {1,2}");
        TestUtils::expect(map.set_passable({3, 2}, true), "[Test2.3] set_passable {3,2}");
        TestUtils::expect(map.set_passable({0, 3}, true), "[Test2.3] set_passable {0,3}");
        TestUtils::expect(map.set_passable({0, 4}, true), "[Test2.3] set_passable {0,4}");
        TestUtils::expect(map.set_passable({1, 4}, true), "[Test2.3] set_passable {1,4}");

        auto dir = Pathfinding::step_towards(map, {0, 2}, {4, 2});
        TestUtils::expect(dir.has_value(), "[Test2.3] step_towards should return a direction");

        if (dir) {
            TestUtils::expect(*dir == Direction::Up, "[Test2.3] expected Up direction");
        }
    }

    //test3
    {
        Map map{5, 5};
        TestUtils::expect(map.set_passable({1, 2}, true), "[Test3] set_passable {1,2}");
        TestUtils::expect(map.set_passable({3, 2}, true), "[Test3] set_passable {3,2}");

        auto dir = Pathfinding::step_towards(map, {1, 2}, {3, 2});
        TestUtils::expect(!dir.has_value(),
                          "[Test3] step_towards should return nullopt when no path exists");
    }

    return TestUtils::fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
