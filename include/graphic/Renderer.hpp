#pragma once

#include <cstddef>
#include <unordered_map>

#include <d2d1.h>
#include <dwrite.h>
#include <wil/com.h>
#include <Eigen/Core>

#include "Brush.hpp"
#include "Theme.hpp"
#include "graphic/TextFormat.hpp"
#include "type/WindowHandle.hpp"
#include "type/BasicTypes.hpp"

namespace gauzy::comp
{
    class Component;
}

namespace gauzy::graphic
{
    /**
     * @brief 渲染器。按窗口句柄指派渲染目标，负责其客户区渲染绘制，封装了 D2D 绘图接口。`gauzy::Window` 构造时将默认创建一个渲染器并管理。
     * @warning 渲染器对象不可拷贝和移动，所有权始终属于初始化者。
     */
    class Renderer
    {
    public:
        /**
         * @brief 按窗口句柄创建渲染器，初始化 D2D 绘图设备。
         */
        explicit Renderer(const type::WindowHandle& handle);

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;
        ~Renderer() = default;

        /**
        * @brief 一般无需主动调用，当所属 `gauzy::Window` 对象窗口尺寸变化时自动更新尺寸。
        */
        void updateSize();

        /**
         * @brief 在客户区渲染组件，调用其 `render` 方法。
         */
        void render(comp::Component& component);

        /**
         * @brief 一般无需主动调用，当所属 `gauzy::Window` 对象窗口销毁前自动释放资源。
         */
        void release();

        /**
         * @brief 获取渲染目标尺寸，单位为像素。
         */
        [[nodiscard]] type::SizeU getSize() const noexcept;

        /**
         * @brief 获取主题对象，决定组件的视觉样式。注意返回的引用是可修改的。
         */
        [[nodiscard]] Theme& theme() noexcept;

        [[nodiscard]] Eigen::Vector2f getDpi() const;

        /**
         * @brief 一般无需主动调用，当所属 `gauzy::Window` 对象 DPI 变化时（ `WM_DPICHANGED` ）自动更新 DPI 缩放比。
         */
        void updateDpi(float dpiX, float dpiY);

        void drawLine(const type::Position2F& start, const type::Position2F& end,
            const Brush& brush) const;

        void drawText(const type::Position2F& position, const std::string& text,
            const Brush& brush, const TextFormat& textFormat) const;
        
        void drawRect(const type::Position2F& topLeft, const type::SizeF& size,
            const Brush& brush) const;
        
        void fillRect(const type::Position2F& topLeft, const type::SizeF& size,
            const Brush& brush) const;

    private:
        type::WindowHandle windowHandle;
        type::SizeU renderTargetSize{ 0U, 0U };
        Theme theme_{ GAUZY_DARK_THEME };

        wil::com_ptr<ID2D1Factory> pD2DFactory;
        wil::com_ptr<ID2D1HwndRenderTarget> pRenderTarget;
        wil::com_ptr<IDWriteFactory> pDWriteFactory;

        mutable std::unordered_map<std::size_t, wil::com_ptr<ID2D1Brush>> brushMap;
        const wil::com_ptr<ID2D1Brush>& getOrCreateBrush(const Brush& brush) const;
        
        mutable std::unordered_map<std::size_t, wil::com_ptr<IDWriteTextFormat>> textFormatCache;
        const wil::com_ptr<IDWriteTextFormat>& getOrCreateTextFormat(const TextFormat& textFormat) const;
    };
} // namespace gauzy::graphic
