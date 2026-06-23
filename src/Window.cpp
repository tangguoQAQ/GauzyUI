#include "Window.hpp"

#include <mutex>
#include <stdexcept>

#include <commctrl.h>
#include <winuser.h>

#include "Util.hpp"

namespace gauzy
{
    constexpr wchar_t CLASS_NAME[] = L"GauzyWindowClass";
    
    static void registerWindowClass(HINSTANCE hInstance)
    {
        
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
        wc.lpszClassName = CLASS_NAME;

        RegisterClassExW(&wc);
    }

    static type::WindowHandle createWindow(Window* thisPtr, const std::string& title, const gauzy::type::SizeU& size)
    {
        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        static std::once_flag gauzyWindowClassFlag;
        std::call_once(gauzyWindowClassFlag, registerWindowClass, hInstance);

        std::wstring wideTitle = util::utf8ToWide(title);
        HWND result = CreateWindowEx(
            0,
            CLASS_NAME,
            wideTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            static_cast<int>(size.x()), static_cast<int>(size.y()),
            nullptr, nullptr, hInstance, nullptr
        );

        if(result == nullptr)
        {
            throw std::runtime_error("Failed to create window");
        }

        return type::WindowHandle{ result };
    }

    Window::Window(const std::string& title, const gauzy::type::SizeU& size) :
        handle{ createWindow(this, title, size) },
        renderer{ handle }
    {
        SetWindowSubclass(static_cast<HWND>(handle), Window::GauzyWindowProc,
            0, reinterpret_cast<LONG_PTR>(this));
    }

    void Window::show() const
    {
        ShowWindow(static_cast<HWND>(handle), SW_SHOW);
        UpdateWindow(static_cast<HWND>(handle));

        MSG msg{};
        while(GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Window::paint()
    {
        renderer.render(topGroup);
    }

    const type::WindowHandle& Window::getWindowHandle() const noexcept
    {
        return handle;
    }

    comp::CompGroup& Window::getTopGroup() noexcept
    {
        return topGroup;
    }

    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    LRESULT Window::GauzyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
            UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
    {
        auto* window = reinterpret_cast<Window*>(dwRefData);      //NOLINT(performance-no-int-to-ptr)

        switch(uMsg)
        {
            case WM_DESTROY:
                window->release();
                PostQuitMessage(0);

                return 0;
            
            case WM_SIZE:
                window->renderer.updateSize();
                break;

            case WM_PAINT:
                window->paint();
                break;
            
            default:
                break;
        }

        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    void Window::release()
    {
        renderer.release();
    }

} // namespace gauzy::comp
