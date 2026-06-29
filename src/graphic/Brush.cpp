#include "graphic/Brush.hpp"

namespace gauzy::graphic
{
    Brush::Brush(const float opacity) noexcept : opacity(opacity)
    {
    }

    bool Brush::operator==(const Brush& rhs) const noexcept
    {
        return opacity == rhs.opacity;
    }

    bool Brush::operator!=(const Brush& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    std::size_t Brush::hash() const noexcept
    {
        std::size_t seed = 0x02E2FD44;
        seed ^= (seed << 6) + (seed >> 2) + 0x71F17FA5 + static_cast<std::size_t>(opacity);
        return seed;
    }

    SolidColorBrush::SolidColorBrush(const type::Color& color) noexcept : color(color)
    {
    }

    bool SolidColorBrush::operator==(const SolidColorBrush& rhs) const noexcept
    {
        return Brush::operator==(rhs) && color == rhs.color;
    }

    bool SolidColorBrush::operator!=(const SolidColorBrush& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    std::size_t SolidColorBrush::hash() const noexcept
    {
        std::size_t seed = 0x1F5FEF24;
        seed ^= (seed << 6) + (seed >> 2) + 0x7AD64F09 + Brush::hash();
        seed ^= (seed << 6) + (seed >> 2) + 0x0D389A09 + color.hash();
        return seed;
    }
}
