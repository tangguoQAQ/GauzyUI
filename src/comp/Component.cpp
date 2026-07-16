#include <utility>

#include "comp/Component.hpp"

namespace gauzy::comp
{
    Component::Component(type::Position2F pos, type::SizeF size) noexcept :
       position(std::move(pos)), size(std::move(size))
    {
    }

    type::Position2F Component::getPosition() const noexcept
    {
        return position;
    }

    void Component::setPosition(type::Position2F newPos) noexcept
    {
        position = std::move(newPos);
    }

    type::SizeF Component::getSize() const noexcept
    {
        return size;
    }

    void Component::setSize(type::SizeF newSize) noexcept
    {
        size = std::move(newSize);
    }


    bool Component::contains(type::Position2F pos) const noexcept
    {
        return pos.x() >= this->position.x()
            && pos.x() <= this->position.x() + this->size.x()
            && pos.y() >= this->position.y()
            && pos.y() <= this->position.y() + this->size.y();
    }
}
