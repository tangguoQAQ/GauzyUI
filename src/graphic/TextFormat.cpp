#include "graphic/TextFormat.hpp"

#include <string>
#include <functional>

namespace gauzy::graphic
{
    bool TextFormat::operator==(const TextFormat& rhs) const noexcept
    {
        return fontFamily == rhs.fontFamily
            && fontSize == rhs.fontSize
            && fontWeight == rhs.fontWeight
            && fontStyle == rhs.fontStyle
            && fontStretch == rhs.fontStretch
            && localeName == rhs.localeName;
    }

    bool TextFormat::operator!=(const TextFormat& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    std::size_t TextFormat::hash() const noexcept
    {
        std::size_t seed = 0x4A8B2C1D;
        seed ^= (seed << 6) + (seed >> 2) + 0x9E3F7A2B + std::hash<std::string_view>{}(fontFamily);
        seed ^= (seed << 6) + (seed >> 2) + 0x1C5E8A3F + static_cast<std::size_t>(fontSize);
        seed ^= (seed << 6) + (seed >> 2) + 0x8B2D4F6A + static_cast<std::size_t>(fontWeight);
        seed ^= (seed << 6) + (seed >> 2) + 0x5F7A9E2B + static_cast<std::size_t>(fontStyle);
        seed ^= (seed << 6) + (seed >> 2) + 0x2A4F8B1D + static_cast<std::size_t>(fontStretch);
        seed ^= (seed << 6) + (seed >> 2) + 0x7E3A5F9C + std::hash<std::string_view>{}(localeName);

        return seed;
    }

} // namespace gauzy::graphic
