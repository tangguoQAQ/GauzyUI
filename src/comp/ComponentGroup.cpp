#include "comp/ComponentGroup.hpp"

namespace gauzy::comp
{
    void gauzy::comp::ComponentGroup::render(graphic::Renderer& renderer)
    {
        for(auto&& child : children)
        {
            child->render(renderer);
        }
    }

    OptionalComponentRef ComponentGroup::getComponentAt(type::Position2F pos)
    {
        if(!contains(pos))
        {
            return std::nullopt;
        }

        for(auto rit = children.rbegin(); rit != children.rend(); ++rit)
        {
            if(auto* group = dynamic_cast<ComponentGroup*>(rit->get()))
            {
                if(auto result = group->getComponentAt(pos))
                {
                    return result;
                }
            }
            else if((*rit)->contains(pos))
            {
                return { *rit->get() };
            }
        }

        if(this->isNotLogicalGroup())
        {
            return { *this };
        }
        return std::nullopt;
    }

    bool gauzy::comp::ComponentGroup::isNotLogicalGroup() const noexcept
    {
        return !std::isinf(getSize().x());
    }

} // namespace gauzy::comp
