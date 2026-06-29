#include "type/Color.hpp"

namespace gauzy::type
{
    Color::Color(const std::uint32_t hex) noexcept : value(hex)
    {
    }

    Color::Color(const std::uint32_t rgb, const std::uint8_t alpha) noexcept :
        value( alpha << 24 | rgb)
    {
    }

    Color::Color(const std::uint8_t r, const std::uint8_t g,
        const std::uint8_t b, const std::uint8_t a) noexcept :
        value(a << 24 | r << 16 | g << 8 | b)
    {
    }

    bool Color::operator==(const Color& rhs) const noexcept
    {
        return value == rhs.value;
    }

    bool Color::operator!=(const Color& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    std::uint8_t Color::r() const noexcept
    {
        return value >> 16 & 255;
    }

    std::uint8_t Color::g() const noexcept
    {
        return value >> 8 & 255;
    }

    std::uint8_t Color::b() const noexcept
    {
        return value & 255;
    }

    std::uint32_t Color::rgb() const noexcept
    {
        return value & 0xFFFFFF;
    }

    std::uint8_t Color::a() const noexcept
    {
        return value >> 24 & 255;
    }

    void Color::r(const std::uint8_t r) noexcept
    {
        value = (value & 0xFF00FFFF) | r << 16;
    }

    void Color::g(const std::uint8_t g) noexcept
    {
        value = (value & 0xFFFF00FF) | g << 8;
    }

    void Color::b(const std::uint8_t b) noexcept
    {
        value = (value & 0xFFFFFF00) | b;
    }

    void Color::rgb(const std::uint32_t rgb) noexcept
    {
        value = (value & 0xFF000000) | rgb;
    }

    void Color::a(const std::uint8_t a) noexcept
    {
        value = (value & 0x00FFFFFF) | a << 24;
    }

    std::size_t Color::hash() const noexcept
    {
        std::size_t seed = 0x07D76507;
        seed ^= (seed << 6) + (seed >> 2) + 0x715BA62E + static_cast<std::size_t>(value);
        return seed;
    }

    Color::operator D2D1::ColorF() const noexcept
    {
        return {rgb(), a() / 255.0f};
    }
}
