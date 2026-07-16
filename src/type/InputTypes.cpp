#include "type/InputTypes.hpp"

#include <stdexcept>

namespace gauzy::type
{
    MouseButtonType toMouseButtonType(const UINT uMsg, WPARAM wParam) noexcept
    {
        switch (uMsg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            return MouseButtonType::Left;
            
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            return MouseButtonType::Right;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            return MouseButtonType::Middle;

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        {
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
            {
                return MouseButtonType::X1;
            }
            return MouseButtonType::X2;
        }
        
        default:
            return MouseButtonType::Left;
        }
    }
}
