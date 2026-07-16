#include "event/EventContext.hpp"

namespace gauzy::event
{
    void EventContext::interrupt() noexcept
    {
        interrupted = true;
    }

    void EventContext::unsubscribeThis() noexcept
    {
        unsubscribed = true;
    }

    void EventContext::resetForNext() noexcept
    {
        interrupted = false;
        unsubscribed = false;
    }

    void EventContext::cancelAction() noexcept
    {
        canceled = true;
    }

    bool EventContext::isCanceled() const noexcept
    {
        return canceled;
    }
}
