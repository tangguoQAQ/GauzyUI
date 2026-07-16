#include "event/WindowMessageDispatcher.hpp"

#include "Window.hpp"
#include "Util.hpp"
#include "comp/Interactive.hpp"
#include "event/ComponentEventContext.hpp"
#include "type/BasicTypes.hpp"

// NOLINTBEGIN(bugprone-easily-swappable-parameters)

namespace gauzy::event
{
    WindowMessageDispatcher::WindowMessageDispatcher(Window& parentWindow) noexcept :
        window{ parentWindow }
    {
    }

    static bool isClickMessage(UINT uMsg) noexcept {
        return uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP
            || uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP
            || uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP
            || uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONUP;
    }

    static bool isMouseButtonDown(UINT uMsg) noexcept {
        return uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN
            || uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN;
    }

    std::optional<LRESULT> WindowMessageDispatcher::dispatch(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(isClickMessage(uMsg))
        {
            dispatchClickMessage(uMsg, wParam, lParam);
            return 0;
        }

        return std::nullopt;
    }

    void WindowMessageDispatcher::dispatchClickMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        const type::Position2F pos = type::Position2F(lParam) / window.get().getDpiScale();
        auto target = window.get().topGroup().getComponentAt(pos);

        if(!target)
        {
            return;
        }
        if(auto* interactive = dynamic_cast<comp::Interactive*>(&target.value().get()))
        {

            event::ClickEventContext e{ { {}, target->get() }, pos, type::toMouseButtonType(uMsg, wParam) };
            
            if(isMouseButtonDown(uMsg))
            {
                interactive->onClicking.trigger(e);
            }
            else
            {
                interactive->onClicked.trigger(e);
            }
        }
    }
}

// NOLINTEND(bugprone-easily-swappable-parameters)