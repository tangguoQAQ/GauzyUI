#pragma once

#include <functional>

namespace gauzy::event
{
    /**
     * @brief 事件上下文。每次事件触发时会创建一个事件上下文对象，并传入每个事件监听器，提供监听器的控制流操作。
     * @see gauzy::event::EventContextWith<T> 提供附带参数对象引用。
     */
    class EventContext
    {
    public:
        EventContext() noexcept = default;
        
        /**
         * @brief 中断调用后续的事件监听器。
         */
        void interrupt() noexcept;

        /**
         * @brief 取消当前事件监听器的订阅，此后不再调用。
         */
        void unsubscribeThis() noexcept;
        
        /**
         * @brief 取消执行默认操作。应当仅用于一些可以取消的预事件，且触发之后应该检查 `isCanceled()` 并处理。
         * 可取消的预事件请在文档中用 `@@note Cancelable` 说明。
         */
        void cancelAction() noexcept;
        
        /**
         * @brief 检查是否取消了执行默认操作。
         */
        [[nodiscard]] bool isCanceled() const noexcept;

    private:
        template <typename EventContextArg>
        friend class Event;

        bool interrupted{ false };
        bool unsubscribed{ false };
        bool canceled{ false };

        void resetForNext() noexcept;
    };

    /**
     * @brief 提供一个附带参数对象 *引用* 的事件上下文。在事件监听器中可以通过 `->` 操作符访问附带对象。
     * 建议用户通过继承 `EventContext` 来实现附带参数以提高可拓展性，初始化请使用聚合初始化。
     * @tparam T 事件上下文附带参数对象类型。
     */
    template <typename T>
    class EventContextWith : public EventContext
    {
    public:
        explicit EventContextWith(T& obj);

        T& getObject() noexcept;

        [[nodiscard]] const T& getObject() const noexcept;

        T* operator->() noexcept;

        const T* operator->() const noexcept;

    private:
        std::reference_wrapper<T> ref;
    };

    template <typename T>
    EventContextWith<T>::EventContextWith(T& obj) : ref(obj)
    {
    }

    template <typename T>
    T& EventContextWith<T>::getObject() noexcept
    {
        return ref;
    }

    template <typename T>
    const T& EventContextWith<T>::getObject() const noexcept
    {
        return ref;
    }

    template <typename T>
    T* EventContextWith<T>::operator->() noexcept
    {
        return &ref.get();
    }

    template <typename T>
    const T* EventContextWith<T>::operator->() const noexcept
    {
        return &ref.get();
    }

}
