#pragma once

#include <Windows.h>

namespace gauzy::type
{
    /**
     * @brief Windows 窗口句柄（HWND）的封装类。
     * 
     */
    class WindowHandle
    {
    public:
        WindowHandle() noexcept;
        explicit WindowHandle(HWND handle) noexcept;

        explicit operator HWND() const noexcept;
    
    private:
        HWND value;
    };

} // namespace gauzy::type