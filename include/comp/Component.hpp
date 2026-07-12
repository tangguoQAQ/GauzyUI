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
        Component() = default;
        Component(const Component &) = default;
        Component(Component &&) = default;
        Component &operator=(const Component &) = default;
        Component &operator=(Component &&) = default;
        virtual ~Component() = default;

        Component(type::Position2F position, type::SizeF size) noexcept;

        [[nodiscard]] type::Position2F getPosition() const noexcept;
        void setPosition(type::Position2F newPosition) noexcept;

        [[nodiscard]] type::SizeF getSize() const noexcept;
        void setSize(type::SizeF newSize) noexcept;

        // TODO: render 是 Action 操作，而不是 Event 事件。相关事件是 RENDER_EVENT PRE/POST。
        virtual void render(graphic::Renderer& renderer) = 0;

    private:
        type::Position2F position;
        type::SizeF size;
    };

} // namespace gauzy::comp
