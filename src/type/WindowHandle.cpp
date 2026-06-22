#include "type/WindowHandle.hpp"

namespace gauzy::type
{
    WindowHandle::WindowHandle() : value(nullptr)
    {
    }
    
    WindowHandle::WindowHandle(HWND handle) : value(handle)
    {
    }

    WindowHandle::operator HWND() const
    {
        return value;
    }
}