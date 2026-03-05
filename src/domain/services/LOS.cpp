#include "domain/services/LOS.hpp"
#include "domain/entities/Map.hpp"

#include "infra/log/Logger.hpp"

#include <cstdint>
#include <cstdlib>

namespace Domain::Services {
bool LOS::can_see(const Domain::Entities::Map &map, const Domain::Core::Position &a,
                  const Domain::Core::Position &b)
{
    LOG(DEBUG) << "LOS::can_see from (" << a.x << "," << a.y << ") to (" << b.x << "," << b.y
               << ")";

    if (!map.in_bounds(a) || !map.in_bounds(b)) {
        LOG(DEBUG) << "LOS::can_see failed: one or both positions out of bounds";
        return false;
    }

    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {

        Domain::Core::Position current_pos{static_cast<std::uint16_t>(x0),
                                           static_cast<std::uint16_t>(y0)};
        if (!map.is_transparent(current_pos)) {
            LOG(DEBUG) << "LOS blocked at (" << current_pos.x << "," << current_pos.y << ")";
            return false;
        }

        if (x0 == x1 && y0 == y1) {
            LOG(DEBUG) << "LOS success: target reached";
            return true;
        }

        int e2 = 2 * err;

        int old_x = x0;
        int old_y = y0;

        if (e2 >= -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }

        if (x0 != old_x && y0 != old_y) {
            Domain::Core::Position side1{static_cast<std::uint16_t>(old_x),
                                         static_cast<std::uint16_t>(y0)};
            Domain::Core::Position side2{static_cast<std::uint16_t>(x0),
                                         static_cast<std::uint16_t>(old_y)};
            if (!map.is_transparent(side1) || !map.is_transparent(side2)) {
                LOG(DEBUG) << "LOS blocked at corner (" << side1.x << "," << side1.y << ") or ("
                           << side2.x << "," << side2.y << ")";
                return false;
            }
        }
    }
}
} // namespace Domain::Services
