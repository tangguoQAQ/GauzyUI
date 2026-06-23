#pragma once

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
        Component &operator=(const Component &) = default;
        Component(Component &&) = default;
        Component &operator=(Component &&) = default;
        virtual ~Component() = default;

        // render 是 Action 操作，而不是 Event 事件。相关事件是 RENDER_EVENT PRE/POST。
        virtual void render(graphic::Renderer& renderer) = 0;
    };

} // namespace gauzy::comp
