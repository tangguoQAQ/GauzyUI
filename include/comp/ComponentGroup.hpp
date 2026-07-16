#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <optional>
#include <functional>
#include <limits>

#include "comp/Component.hpp"

namespace gauzy::comp
{
    using OptionalComponentRef = std::optional<std::reference_wrapper<Component>>;

    /**
     * @brief 组件群。对多个组件进行 *逻辑分组* 管理，没有额外视觉效果，
     * 因而默认尺寸分量均设置为 `std::numeric_limits<float>::infinity()`，默认位置分量均设置为 `std::numeric_limits<float>::lowest()`，
     * 且不应更改，但视觉分组的派生类请正确设置。
     */
    class ComponentGroup : public Component
    {
    public:
        ComponentGroup(const ComponentGroup& other) = delete;
        ComponentGroup(ComponentGroup&& other) noexcept = delete;
        ComponentGroup& operator=(const ComponentGroup& other) = delete;
        ComponentGroup& operator=(ComponentGroup&& other) noexcept = delete;
        ~ComponentGroup() override = default;

        /**
         * @brief 构造组件群并初始化子组件。
         * 
         * @tparam Comps 每个类型都必须以 Component 类型为基类。
         * @param newChildren 待加入组件，可以是多个，也可为空。
         * 
         * @warning 当传入右值时，如构造返回的临时对象 `ComponentGroup(SomeComp{...})` ，会移动存入该组件群，这是一般的做法。但如果传入左值
         * 引用，如 `ComponentGroup(someComp)` ，则会 *复制* 存入该组件群，要注意未来对原对象的修改不会同步到已存入群中的组件。
         */
        template <typename... Comps>
        explicit ComponentGroup(Comps&&... newChildren);

        /**
         * @brief 向群中添加组件。
         * 
         * @tparam Comps 每个类型都必须以 Component 类型为基类。
         * @param newChildren 待加入组件，可以是多个，但不可为空。
         * 
         * @warning 当传入右值时，如构造返回的临时对象 `group.add(SomeComp{...})` ，会移动存入该组件群，这是一般的做法。但如果传入左值
         * 引用，如 `group.add(someComp)` ，则会 *复制* 存入该组件群，要注意未来对原对象的修改不会同步到已存入群中的组件。
         */
        template <typename... Comps>
        void add(Comps&&... newChildren);

        void render(graphic::Renderer& renderer) override;

        /**
         * @brief 获取指定坐标下最深层组件。递归遍历子组件进行命中测试。
         * @return 确保返回值一定不是 ComponentGroup（逻辑分组），但可能是实现了视觉分组且指定位置没有子组件的派生类（e.g. Panel）。
         * 如果没有找到，返回 `std::nullopt`。
         */
        [[nodiscard]] OptionalComponentRef getComponentAt(type::Position2F pos);

    private:
        std::vector<std::unique_ptr<Component>> children;

        [[nodiscard]] bool isNotLogicalGroup() const noexcept;
    };

    template <typename... Comps>
    ComponentGroup::ComponentGroup(Comps&&... newChildren) :
        Component(
            { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() },
            { std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() })
    {
        if constexpr(sizeof...(newChildren) != 0)
        {
            add(std::forward<Comps>(newChildren)...);
        }
    }

    template <typename... Comps>
    void ComponentGroup::add(Comps&&... newChildren)
    {
        static_assert(sizeof...(newChildren) > 0,
                      "The parameters passed to ComponentGroup::add() must not be empty.");
        static_assert((std::is_base_of_v<Component, std::decay_t<Comps>> && ...),
                      "All parameters passed to ComponentGroup::add() must be derived from Component.");

        (children.push_back(std::make_unique<std::decay_t<Comps>>(std::forward<Comps>(newChildren))), ...);
    }
} // namespace gauzy::comp
