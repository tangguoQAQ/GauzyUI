#pragma once

#include <string>

#include "type/BasicTypes.hpp"
#include "type/WindowHandle.hpp"
#include "graphic/Renderer.hpp"

namespace gauzy
{
    /**
     * @brief 薄云窗口类。窗口类名为 `GauzyWindowClass` 。
     */
    class Window
    {
    public:
        Window(const std::string& title, const gauzy::type::SizeU& size);
        
        void show() const;

        void release();

        [[nodiscard]] type::WindowHandle getWindowHandle() const;

        /**
         * @brief 获取窗口所管理的渲染器。注意返回值为引用，见 `graphic::Renderer` 。
         */
        [[nodiscard]] graphic::Renderer& getRenderer();

    private:
        type::WindowHandle handle;

        graphic::Renderer renderer;
    };
} // namespace gauzy
