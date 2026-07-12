#pragma once

#include <functional>

#include "type/Color.hpp"

namespace gauzy::graphic
{
    /**
     * @brief 画刷基类。画刷决定图形元素的绘制样式，如颜色、图像和透明度。
     */
    struct Brush
    {
        float opacity{ 1.0F };
        // TODO: D2D1_MATRIX_3X2_F transform;

        Brush(const Brush& other) = default;
        Brush(Brush&& other) noexcept = default;
        Brush& operator=(const Brush& other) = default;
        Brush& operator=(Brush&& other) noexcept = default;
        virtual ~Brush() = default;

        explicit Brush(float opacity = 1.0F) noexcept;

        virtual bool operator==(const Brush& rhs) const noexcept;
        virtual bool operator!=(const Brush& rhs) const noexcept;

        [[nodiscard]] virtual std::size_t hash() const noexcept;

        [[nodiscard]] virtual D2D1_BRUSH_PROPERTIES toD2DProperties() const;
    };

    /**
     * @brief 实色画刷。见 `Brush` 。
     */
    struct SolidColorBrush : Brush
    {
        type::Color color;

        explicit SolidColorBrush(const type::Color& color) noexcept;

        virtual bool operator==(const SolidColorBrush& rhs) const noexcept;
        virtual bool operator!=(const SolidColorBrush& rhs) const noexcept;

        [[nodiscard]] std::size_t hash() const noexcept override;
    };

    /**
     * @brief 线性渐变画刷。见 `Brush` 。
     */
    struct LinearGradientBrush : Brush
    {
        // TODO
    };

    /**
     * @brief 径向渐变画刷。见 `Brush` 。
     */
    struct RadialGradientBrush : Brush
    {
        // TODO
    };

    /**
     * @brief 图像画刷。见 `Brush` 。
     */
    struct ImageBrush : Brush
    {
        // TODO
    };

    // TODO: 纹理画刷 / 图案画刷？
} // namespace gauzy::graphic
