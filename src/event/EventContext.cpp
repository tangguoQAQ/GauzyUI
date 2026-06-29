#include "event/EventContext.hpp"

namespace gauzy::event
{
    void EventContext::interrupt()
    {
        interrupted = true;
    }

    void EventContext::unsubscribeThis()
    {
        unsubscribed = true;
    }

    void EventContext::next()
    {
        unsubscribed = false;
    }

}
