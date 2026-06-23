#pragma once

#include <string>

#include <Windows.h>

#include "type/BasicTypes.hpp"
#include "type/WindowHandle.hpp"
#include "graphic/Renderer.hpp"
#include "comp/CompGroup.hpp"

namespace gauzy
{
    /**
     * @brief 薄云窗口类。窗口类名为 `GauzyWindowClass` 。
     */
    class Window
    {
    public:
        Window(const std::string& title, const gauzy::type::SizeU& size);
        
        /**
         * @brief 显示窗口并进入消息循环，此函数会将阻塞直到窗口关闭。
         */
        void show() const;

        /**
         * @brief 重画窗口客户区。
         */
        void paint();

        [[nodiscard]] const type::WindowHandle& getWindowHandle() const noexcept;

        /**
        * @brief 获取窗口的顶级组件群。
        */
        [[nodiscard]] comp::CompGroup& getTopGroup() noexcept;

    private:
        type::WindowHandle handle;

        graphic::Renderer renderer;

        comp::CompGroup topGroup;

        static LRESULT GauzyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
            UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        void release();
    };
} // namespace gauzy
