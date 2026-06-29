#pragma once

#include <functional>

#include "type/Color.hpp"

namespace gauzy::graphic
{
    /**
     * @brief 画刷基类。使用前必须监听对应窗口的 `onRendererRegistry` 事件并注册画刷，注册的画刷直到其所属渲染器析构时才被销毁。
     * @details 画刷决定了图形元素的绘制样式，如颜色、图像和透明度。
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

template <>
struct std::hash<gauzy::graphic::Brush>
{
    std::size_t operator()(const gauzy::graphic::Brush& brush) const noexcept
    {
        return brush.hash();
    }
};
