#include "comp/CompGroup.hpp"

namespace gauzy::comp
{
    void gauzy::comp::CompGroup::render(graphic::Renderer& renderer)
    {
        for(auto&& child : children)
        {
            child->render(renderer);
        }
        
    }
} // namespace gauzy::comp
