#include <utility>

#include "comp/Component.hpp"

namespace gauzy::comp
{
    Component::Component(type::Position2F position, type::SizeF size) noexcept :
       position(std::move(position)), size(std::move(size))
    {
    }

    type::Position2F Component::getPosition() const noexcept
    {
        return position;
    }

    void Component::setPosition(type::Position2F newPosition) noexcept
    {
        position = std::move(newPosition);
    }

    type::SizeF Component::getSize() const noexcept
    {
        return size;
    }

    void Component::setSize(type::SizeF newSize) noexcept
    {
        size = std::move(newSize);
    }
}
