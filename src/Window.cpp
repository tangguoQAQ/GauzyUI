#include "Window.hpp"

#include <stdexcept>

#include "Util.hpp"

namespace gauzy::comp
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

    Window::Window(const std::string& title, gauzy::type::Vec2d size)
    {
        constexpr wchar_t CLASS_NAME[] = L"GauzyWindowClass";
        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);;
        wc.lpszClassName = CLASS_NAME;

        RegisterClassExW(&wc);

        std::wstring wideTitle = util::utf8ToWide(title);
        handle = CreateWindowEx(
            0,
            CLASS_NAME,
            wideTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            static_cast<int>(size.x), static_cast<int>(size.y),
            nullptr, nullptr, hInstance, nullptr
        );

        if(handle.value == nullptr)
        {
            throw std::runtime_error("Failed to create window");
        }
    }

    void Window::show() const
    {
        ShowWindow(handle, SW_SHOW);
        UpdateWindow(handle);

        MSG msg{};
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

} // namespace gauzy::comp
