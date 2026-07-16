#pragma once

#include <functional>
#include <optional>

#include <Windows.h>

namespace gauzy
{
    class Window;
}

namespace gauzy::event
{
    /**
     * @brief 薄云窗口消息调度器。所有窗口消息都会通过此派发器派发给框架处理。
     * 如果需要编写自定义的消息处理逻辑，见 `Window::onMessageDispatching` / `Window::onMessageDispatched`。
     */
    class WindowMessageDispatcher
    {
    public:
        WindowMessageDispatcher(Window& window) noexcept;

        std::optional<LRESULT> dispatch(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    private:
        std::reference_wrapper<Window> window;

        void dispatchClickMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}