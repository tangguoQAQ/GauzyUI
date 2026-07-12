#pragma once

#include <functional>

namespace gauzy::event
{
    /**
     * @brief 事件上下文。每次事件触发时会创建一个事件上下文对象，并传入每个事件监听器，提供监听器的控制流操作。
     * @see gauzy::event::EventContextWith<T> 提供附带对象引用
     */
    class EventContext
    {
    public:
        /**
         * @brief 中断调用后续的事件监听器。
         */
        void interrupt();

        /**
         * @brief 取消当前事件监听器的订阅，此后不再调用。
         */
        void unsubscribeThis();

    private:
        template <typename EventArg>
        friend class Event;

        bool interrupted{ false };
        bool unsubscribed{ false };

        void next();
    };

    /**
     * @brief 提供一个附带对象 *引用* 的事件上下文。
     * @tparam T 事件上下文附带对象类型。
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
