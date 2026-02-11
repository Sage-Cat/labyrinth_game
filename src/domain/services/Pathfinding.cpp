#include "domain/services/Pathfinding.hpp"
#include "domain/entities/Map.hpp"

#include "infra/log/Logger.hpp"

#include <optional>
#include <queue>
#include <vector>

using Domain::Core::Direction;
using Domain::Core::Position;

namespace Domain::Services {

std::optional<Domain::Core::Direction> Pathfinding::step_towards(const Domain::Entities::Map &map,
                                                                 Domain::Core::Position from,
                                                                 Domain::Core::Position to)
{
    LOG(DEBUG) << "Pathfinding::step_towards from (" << from.x << "," << from.y << ") to (" << to.x
               << "," << to.y << ")";

    if (from == to)
    {
        LOG(DEBUG) << "Pathfinding: start equals target, no movement needed";
        return std::nullopt;
    }

    if (!map.in_bounds(from) || !map.in_bounds(to))
    {
        LOG(DEBUG) << "Pathfinding: one or both positions out of bounds";
        return std::nullopt;
    }
    if (!map.is_passable(from) || !map.is_passable(to))
    {
        LOG(DEBUG) << "Pathfinding: start or target is not passable";
        return std::nullopt;
    }


    const auto w = map.width();
    const auto h = map.height();
    auto idx     = [w](Position p) { return std::size_t(p.y) * w + p.x; };

    std::vector<std::optional<Position>> parent(w * h);
    std::queue<Position> q;

    parent[idx(from)] = from;
    q.push(from);

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();

        std::vector<Position> neighbors;

        if (cur.y > 0)
            neighbors.push_back({cur.x, static_cast<uint16_t>(cur.y - 1)});

        if (cur.y + 1 < map.height())
            neighbors.push_back({cur.x, static_cast<uint16_t>(cur.y + 1)});

        if (cur.x > 0)
            neighbors.push_back({static_cast<uint16_t>(cur.x - 1), cur.y});

        if (cur.x + 1 < map.width())
            neighbors.push_back({static_cast<uint16_t>(cur.x + 1), cur.y});

        for (const auto &next : neighbors) {
            if (!map.in_bounds(next))
                continue;
            if (!map.is_passable(next))
                continue;
            if (parent[idx(next)].has_value())
                continue;

            parent[idx(next)] = cur;
            q.push(next);
        }
    }

    if (!parent[idx(to)].has_value())
    {
        LOG(DEBUG) << "Pathfinding: no path found to target";
        return std::nullopt;
    }

    Position step = to;
    while (parent[idx(step)].value() != from)
        step = parent[idx(step)].value();
    if (step.x > from.x)
    {
        LOG(DEBUG) << "Pathfinding: next step is Right";
        return Direction::Right;
    }
  
    if (step.x < from.x)
    {
        LOG(DEBUG) << "Pathfinding: next step is Left";
        return Direction::Left;
    }

    if (step.y > from.y)
    {
        LOG(DEBUG) << "Pathfinding: next step is Down";
        return Direction::Down;
    }

    if (step.y < from.y)
    {
        LOG(DEBUG) << "Pathfinding: next step is Up";
        return Direction::Up;
    }

    LOG(DEBUG) << "Pathfinding: no movement direction resolved";
    return std::nullopt;
}

} // namespace Domain::Services
