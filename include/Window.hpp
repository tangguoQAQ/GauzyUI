#pragma once

#include <string>

#include <Windows.h>

#include "type/Vec2d.hpp"

namespace gauzy::comp
{
    struct WindowHandle
    {
        HWND value;

        WindowHandle();
        WindowHandle(HWND handle);

        operator HWND() const;
    };

    class Window
    {
    public:
        Window(const std::string& title, gauzy::type::Vec2d size);
        
        void show() const;

        [[nodiscard]] WindowHandle getWindowHandle() const;

    private:
        WindowHandle handle;

    };
} // namespace gauzy::comp
