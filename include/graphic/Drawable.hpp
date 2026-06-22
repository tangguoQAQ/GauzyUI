#pragma once

#include "type/BasicTypes.hpp"

namespace gauzy::graphic
{
    /**
     * @brief 绘图接口。提供基础的绘制图形、文本等接口。见 `gauzy::graphic::Renderer`。
     */
    class Drawable
    {
    public:
        // fucking long specific functions
        Drawable() = default;
        Drawable(const Drawable &) = delete;
        Drawable(Drawable &&) = delete;
        Drawable &operator=(const Drawable &) = delete;
        Drawable &operator=(Drawable &&) = delete;
        virtual ~Drawable() = default;

        virtual void drawLine(const type::Position2F &start, const type::Position2F &end) = 0;
    };

} // namespace gauzy::graphic