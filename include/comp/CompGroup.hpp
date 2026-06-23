#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include "comp/Component.hpp"

namespace gauzy::comp
{
    /**
     * @brief 组件群。对多个组件进行 *逻辑分组* 管理，没有额外视觉效果。
     */
    class CompGroup : public Component
    {
    public:
        /**
         * @brief 构造组件群并初始化子组件。
         * 
         * @tparam Comps 每个类型都必须以 Component 类型为基类。
         * @param newChildren 带加入组件，可以是多个，也可为空。
         * 
         * @warning 当传入右值时，如构造返回的临时对象 `CompGroup(SomeComp{...})` ，会移动存入该组件群，这是一般的做法。但如果传入左值引用，如 `CompGroup(someComp)` ，则会 *复制* 存入该组件群，要注意未来对原对象的修改不会同步已存入群中的组件。
         */
        template<typename... Comps>
        CompGroup(Comps&&... newChildren);

        ~CompGroup() override = default;

        /**
         * @brief 向群中添加组件。
         * 
         * @tparam Comps 每个类型都必须以 Component 类型为基类。
         * @param newChildren 带加入组件，可以是多个，但不可为空。
         * 
         * @warning 当传入右值时，如构造返回的临时对象 `group.add(SomeComp{...})` ，会移动存入该组件群，这是一般的做法。但如果传入左值引用，如 `group.add(someComp)` ，则会 *复制* 存入该组件群，要注意未来对原对象的修改不会同步已存入群中的组件。
         */
        template<typename... Comps>
        void add(Comps&&... newChildren);

        void render(graphic::Renderer& renderer) override;
    
    private:
        std::vector<std::unique_ptr<Component>> children;
    };

    template<typename... Comps>
    inline CompGroup::CompGroup(Comps&&... newChildren)
    {
        if constexpr(sizeof...(newChildren) != 0)
        {
            add(std::forward(newChildren)...);
        }
    }

    template<typename... Comps>
    inline void CompGroup::add(Comps&&... newChildren)
    {
        static_assert(sizeof...(newChildren) > 0,
            "The parameters passed to CompGroup::add() must not be empty.");
        static_assert((std::is_base_of_v<Component, std::decay_t<Comps>> && ...),
            "All parameters passed to CompGroup::add() must be derived from Component.");
        
        (children.push_back(std::make_unique<std::decay_t<Comps>>(std::forward(newChildren))), ...);
    }

} // namespace gauzy::comp
