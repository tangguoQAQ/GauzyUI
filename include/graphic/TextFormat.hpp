#pragma once

#include <cstddef>
#include <string>

#include <dwrite.h>

namespace gauzy::graphic
{
    static constexpr float DEFAULT_FONT_SIZE = 12.0F;

    /**
     * @brief 文本格式。用于设置字体的系列、大小、粗细、样式、拉伸等属性。
     * 
     */
    struct TextFormat
    {
        /**
         * @brief 字体系列，或字体族，如 "Segoe UI"、"Microsoft YaHei" 等。
         */
        std::string_view fontFamily{ "Segoe UI" };
        float fontSize{ DEFAULT_FONT_SIZE };
        /**
         * @brief 字体粗细，或字重。
         */
        DWRITE_FONT_WEIGHT fontWeight{ DWRITE_FONT_WEIGHT_NORMAL };
        /**
         * @brief 字体样式，如斜体。
         */
        DWRITE_FONT_STYLE fontStyle{ DWRITE_FONT_STYLE_NORMAL };
        /**
         * @brief 字体拉伸，表示与字体的正常纵横比相比，字体的横向拉伸程度。
         * 
         */
        DWRITE_FONT_STRETCH fontStretch{ DWRITE_FONT_STRETCH_NORMAL };

        /**
         * @brief 区域名称。不同区域设置 `fontFamily` 的本地化名称不同。
         */
        std::string_view localeName{ "zh-CN" };

        TextFormat() noexcept = default;
        TextFormat(const TextFormat&) noexcept = default;
        TextFormat(TextFormat&&) noexcept = default;
        TextFormat& operator=(const TextFormat&) noexcept = default;
        TextFormat& operator=(TextFormat&&) noexcept = default;
        ~TextFormat() noexcept = default;

        bool operator==(const TextFormat& rhs) const noexcept;
        bool operator!=(const TextFormat& rhs) const noexcept;

        [[nodiscard]] std::size_t hash() const noexcept;
    };
} // namespace gauzy::graphic
