#pragma once

#include <string>

#include <Windows.h>

#include "type/BasicTypes.hpp"
#include "type/WindowHandle.hpp"
#include "graphic/Renderer.hpp"
#include "graphic/Theme.hpp"
#include "comp/ComponentGroup.hpp"
#include "event/Event.hpp"
#include "event/WindowEventContext.hpp"
#include "event/WindowMessageDispatcher.hpp"

namespace gauzy
{
    /**
     * @brief 薄云窗口类。窗口类名为 `GauzyWindowClass` 。
     */
    class Window
    {
    public:
        /**
         * @brief 窗口消息广播预事件。在窗口生命周期消息处理完后、框架消息处理前触发。
         * 通过订阅该事件监听消息循环的窗口消息，可用于设置钩子（Hook）或派发到自定义事件。
         * @warning 请谨慎编写监听器代码，危险操作可能会影响框架代码的正常运行。
         * @note Cancelable 此事件可取消框架的消息调度操作，取消后的后事件仍会触发。
         */
        event::Event<event::WindowMessageEventContext> onMessageDispatching;
        /**
         * @brief 窗口消息广播后事件。在窗口生命周期消息、框架消息处理后触发。
         */
        event::Event<event::WindowMessageEventContext> onMessageDispatched;

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        virtual ~Window();

        Window(const std::string& title, const type::SizeU& size);

        /**
         * @brief 显示窗口并进入消息循环，此函数会将阻塞直到窗口关闭。
         */
        void show();

        /**
         * @brief 重画窗口客户区。
         */
        void paint();

        [[nodiscard]] const type::WindowHandle& getWindowHandle() const noexcept;

        [[nodiscard]] const type::SizeU& getSize() const noexcept;

        [[nodiscard]] type::SizeU getClientSize() const;

        [[nodiscard]] float getDpiScale() const noexcept;

        /**
         * @brief 获取主题对象，决定组件的视觉样式。注意返回的引用是可修改的。
         */
        [[nodiscard]] graphic::Theme& theme() noexcept;

        /**
        * @brief 获取窗口的顶级组件群。
        */
        [[nodiscard]] comp::ComponentGroup& topGroup() noexcept;

    private:
        type::WindowHandle handle;
        type::SizeU size;
        float dpiScale;

        event::WindowMessageDispatcher messageDispatcher { *this };
        graphic::Renderer renderer;

        comp::ComponentGroup topGroup_;

        static LRESULT GauzyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
            UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        void release();
    };
} // namespace gauzy
