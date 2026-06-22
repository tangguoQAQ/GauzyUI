#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include "type/WindowHandle.hpp"
#include "type/BasicTypes.hpp"
#include "graphic/Drawable.hpp"

namespace gauzy::graphic
{
    /**
     * @brief 渲染器。按窗口句柄指派渲染目标，负责其客户端渲染绘制，封装了 D2D 绘图接口。`gauzy::Window` 构造时将默认创建一个渲染器并管理。
     * @warning 渲染器对象不可拷贝和移动，所有权始终属于初始化者。
     */
    class Renderer : public Drawable
    {
    public:
        /**
         * @brief 按窗口句柄创建渲染器，初始化 D2D 绘图设备。
         */
        explicit Renderer(type::WindowHandle handle);

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;
        ~Renderer() override = default;

        /**
        * @brief 一般无需主动调用，当所属 `gauzy::Window` 对象窗口尺寸变化时自动更新尺寸。
        */
        void updateSize();
        
        /**
         * @brief 一般无需主动调用，当所属 `gauzy::Window` 对象窗口销毁前自动释放资源。
         */
        void release();

        /**
         * @brief 获取渲染目标尺寸，单位为像素。
         */
        [[nodiscard]] type::SizeU getSize() const;

        void drawLine(const type::Position2F &start, const type::Position2F &end) override;

    private:
        type::WindowHandle windowHandle;
        type::SizeU renderTargetSize{ 0, 0 };

        ID2D1Factory* pD2DFactory{ nullptr };
        ID2D1HwndRenderTarget* pRenderTarget{ nullptr };
        IDWriteFactory* pDWriteFactory{ nullptr };
    };

} // namespace gauzy::graphic