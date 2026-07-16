#pragma once

#include "type/BasicTypes.hpp"

namespace gauzy::graphic
{
    class Renderer;
}

namespace gauzy::comp
{
    /**
     * @brief 薄云组件类。所有组件的基类。 `Component` 的缩写为 `Comp` 。
     */
    class Component
    {
    public:
        Component() noexcept = default;
        Component(const Component &) noexcept = default;
        Component(Component &&) noexcept = default;
        Component &operator=(const Component &) noexcept = default;
        Component &operator=(Component &&) noexcept = default;
        virtual ~Component() = default;

        Component(type::Position2F pos, type::SizeF size) noexcept;

        /// @note 位置是相对于窗口客户区左上角的 DIP。
        [[nodiscard]] type::Position2F getPosition() const noexcept;
        void setPosition(type::Position2F newPos) noexcept;

        [[nodiscard]] type::SizeF getSize() const noexcept;
        void setSize(type::SizeF newSize) noexcept;

        /**
         * @brief 检测位置是否在组件界限内，用于鼠标命中测试等。默认检测矩形界限且包含边界，若需自定义规则请覆写该函数。
         */
        [[nodiscard]] virtual bool contains(type::Position2F pos) const noexcept;

        // TODO: render 是 Action 操作，而不是 Event 事件。
        virtual void render(graphic::Renderer& renderer) = 0;

    private:
        type::Position2F position;
        type::SizeF size;
    };

} // namespace gauzy::comp
