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
        HWND value;
        
        WindowHandle();
        explicit WindowHandle(HWND handle);

        explicit operator HWND() const;
    };

} // namespace gauzy::type