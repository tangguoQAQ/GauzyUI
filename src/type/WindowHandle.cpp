#include "type/WindowHandle.hpp"

namespace gauzy::type
{
    WindowHandle::WindowHandle() noexcept : value(nullptr)
    {
    }
    
    WindowHandle::WindowHandle(HWND handle) noexcept : value(handle)
    {
    }

    WindowHandle::operator HWND() const noexcept
    {
        return value;
    }
}