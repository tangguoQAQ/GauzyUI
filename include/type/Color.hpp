#pragma once

#include <cstddef>
#include <cstdint>

#include <d2d1.h>

namespace gauzy::type
{
    /**
     * @brief 颜色类型。使用32位无符号整数存储颜色值，支持透明度 Alpha 通道。
     */
    struct Color
    {
        /**
         * @brief 格式为 0xAARRGGBB 的颜色值。
         */
        std::uint32_t value;

        explicit Color(std::uint32_t hex = 0xFF000000) noexcept;
        Color(std::uint32_t rgb, float alpha) noexcept;
        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) noexcept;

        bool operator==(const Color& rhs) const noexcept;
        bool operator!=(const Color& rhs) const noexcept;

        [[nodiscard]] std::uint8_t r() const noexcept;
        [[nodiscard]] std::uint8_t g() const noexcept;
        [[nodiscard]] std::uint8_t b() const noexcept;
        /**
         * @brief 获取 0xRRGGBB 颜色值，不含 Alpha 通道。
         */
        [[nodiscard]] std::uint32_t rgb() const noexcept;
        [[nodiscard]] std::uint8_t a() const noexcept;

        void r(std::uint8_t r) noexcept;
        void g(std::uint8_t g) noexcept;
        void b(std::uint8_t b) noexcept;
        /**
         * @brief 设置 0xRRGGBB 颜色值，不改变 Alpha 分量。
         */
        void rgb(std::uint32_t rgb) noexcept;
        void a(std::uint8_t a) noexcept;

        [[nodiscard]] std::size_t hash() const noexcept;

        [[nodiscard]] explicit operator D2D1::ColorF() const noexcept;
    };
}
