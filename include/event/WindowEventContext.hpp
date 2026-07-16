#pragma once

#include <functional>

#include <Windows.h>

#include "event/EventContext.hpp"

namespace gauzy
{
    class Window;
}

namespace gauzy::event
{
    struct WindowEventContext : public EventContext
    {
        std::reference_wrapper<Window> window;
    };

    struct WindowMessageEventContext : public WindowEventContext
    {
        UINT uMsg{};
        WPARAM wParam{};
        LPARAM lParam{};
    };
}