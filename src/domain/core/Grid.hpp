#pragma once
#include <vector>
#include <cstddef>

namespace Domain::Core
{
    template <class T>
    class Grid
    {
    public:
        Grid() = default;
        Grid(std::size_t w, std::size_t h) : w_{w}, h_{h}, cells_(w * h) {}
        [[nodiscard]] std::size_t width() const { return w_; }
        [[nodiscard]] std::size_t height() const { return h_; }
        [[nodiscard]] bool in_bounds(std::size_t x, std::size_t y) const
        {
            return x < w_ && y < h_;
        }
        T &at(std::size_t x, std::size_t y) { return cells_[y * w_ + x]; }
        const T &at(std::size_t x, std::size_t y) const { return cells_[y * w_ + x]; }

    private:
        std::size_t w_{0}, h_{0};
        std::vector<T> cells_;
    };
}
