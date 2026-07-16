#pragma once

#include <functional>

#include "comp/Component.hpp"
#include "type/InputTypes.hpp"
#include "event/EventContext.hpp"

namespace gauzy::event
{
    struct ComponentEventContext : public EventContext
    {
        std::reference_wrapper<comp::Component> target;
    };

    struct ClickEventContext : public ComponentEventContext
    {
        type::Position2F clickPosition;
        type::MouseButtonType buttonType{ type::MouseButtonType::Left };
    };
}