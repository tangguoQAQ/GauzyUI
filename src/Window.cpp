#include "Window.hpp"

#include <mutex>
#include <stdexcept>
#include <cmath>

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
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
        wc.lpszClassName = CLASS_NAME;

        RegisterClassExW(&wc);
    }

    static type::WindowHandle createWindow(const std::string& title, const type::SizeU& size)
    {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        static std::once_flag gauzyWindowClassFlag;
        std::call_once(gauzyWindowClassFlag, registerWindowClass, hInstance);

        const std::wstring wideTitle = util::utf8ToWide(title);
        const float dpiScale = static_cast<float>(GetDpiForSystem()) / 96.0F;
        // NOLINTNEXTLINE(cppcoreguidelines-narrowing-conversions, bugprone-narrowing-conversions)
        const int pixelWidth  = std::lroundf(size.x() * dpiScale);
        // NOLINTNEXTLINE(cppcoreguidelines-narrowing-conversions, bugprone-narrowing-conversions)
        const int pixelHeight = std::lroundf(size.y() * dpiScale);
        const HWND result = CreateWindowEx(
            0,
            CLASS_NAME,
            wideTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            pixelWidth, pixelHeight,
            nullptr, nullptr, hInstance, nullptr
        );

        if(result == nullptr)
        {
            throw std::runtime_error("Failed to create window");
        }

        return type::WindowHandle{ result };
    }

    Window::Window(const std::string& title, const type::SizeU& size) :
        handle{ createWindow(title, size) },
        size{ size },
        renderer{ handle }
    {
        SetWindowSubclass(static_cast<HWND>(handle), GauzyWindowProc,
            0, reinterpret_cast<LONG_PTR>(this));
    }

    void Window::show()
    {
        onRendererRegistry.trigger(event::EventContextWith{ renderer });

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

    const type::SizeU& Window::getSize() const noexcept
    {
        return size;
    }

    type::SizeU Window::getClientSize() const
    {
        RECT rect{};
        GetClientRect(static_cast<HWND>(handle), &rect);

        return type::SizeU{ rect.right - rect.left, rect.bottom - rect.top };
    }

    graphic::Theme& Window::theme() noexcept
    {
        return renderer.theme();
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
            {
                window->release();
                PostQuitMessage(0);

                return 0;
            }
            case WM_SIZE:
            {
                window->renderer.updateSize();
                break;
            }
            case WM_PAINT:
            {
                window->paint();
                break;
            }
            case WM_DPICHANGED:
            {
                const auto dpiX = static_cast<float>(LOWORD(wParam));
                const auto dpiY = static_cast<float>(HIWORD(wParam));
                window->renderer.updateDpi(dpiX, dpiY);

                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
                const auto* rect = reinterpret_cast<RECT*>(lParam);
                SetWindowPos(hWnd, nullptr,
                    rect->left, rect->top,
                    rect->right - rect->left,
                    rect->bottom - rect->top,
                    SWP_NOZORDER | SWP_NOACTIVATE);
                break;
            }
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
