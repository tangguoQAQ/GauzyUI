#pragma once

#include "type/Vec2d.hpp"

namespace gauzy::graphic
{
    /**
     * @brief 绘图接口
     * @details 提供基础的绘制图形、文本等接口。实现见 `gauzy::graphic::Renderer`。
     */
    class Drawable
    {
    public:
      Drawable(const Drawable &) = delete;
      Drawable(Drawable &&) = delete;
      Drawable &operator=(const Drawable &) = delete;
      Drawable &operator=(Drawable &&) = delete;
      virtual ~Drawable() = default;

      virtual void DrawLine(const type::Point &start, const type::Point &end) = 0;
    };

} // namespace gauzy::graphic