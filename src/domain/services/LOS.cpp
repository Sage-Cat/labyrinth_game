#include "domain/services/LOS.hpp"
#include "domain/entities/Map.hpp"
#include <cstdint>
#include <cstdlib>

namespace Domain::Services {
bool LOS::can_see(const Domain::Entities::Map &map, const Domain::Core::Position &a,
                  const Domain::Core::Position &b)
{
    if (!map.in_bounds(a) || !map.in_bounds(b))
        return false;

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

        Domain::Core::Position current_pos{static_cast<std::uint8_t>(x0),
                                           static_cast<std::uint8_t>(y0)};
        if (!map.is_transparent(current_pos))
            return false;

        if (x0 == x1 && y0 == y1)
            return true;

        int e2 = 2 * err;

        if (e2 >= -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}
} // namespace Domain::Services
