#pragma once

#include "event/Event.hpp"
#include "event/ComponentEventContext.hpp"

namespace gauzy::comp
{
    /**
     * @brief 可交互组件接口。实现该接口的组件才会接收鼠标或键盘的输入事件。
     */
    class Interactive
    {
    public:
        Interactive() = default;
        Interactive(const Interactive&) = default;
        Interactive(Interactive&&) noexcept = default;
        Interactive& operator=(const Interactive&) = default;
        Interactive& operator=(Interactive&&) noexcept = default;
        virtual ~Interactive() = default;

        /// @brief 鼠标键按下事件。
        event::Event<event::ClickEventContext> onClicking;
        /// @brief 鼠标键抬起事件。
        event::Event<event::ClickEventContext> onClicked;
    };
}