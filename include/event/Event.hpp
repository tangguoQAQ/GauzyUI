#pragma once

#include <cstdint>
#include <functional>
#include <list>

#include "EventContext.hpp"

namespace gauzy::event
{
    /**
     * @brief 事件类。
     * @details 自定义事件请在事件源的公开成员变量中声明，命名为 `on...` ，使用 Doing 表示预事件、Done 表示后事件，
     * 并在适当位置调用 `trigger` 函数触发。
     * @tparam EventArg 事件上下文参数类型。必须是 EventContext 及其子类（如 EventContextWith），将在事件触发时传递给监听器。
     */
    template <typename EventArg>
    class Event
    {
        static_assert(std::is_base_of_v<EventContext, EventArg>,
                      "EventArg must be derived from EventContext.");

    public:
        Event() = default;
        Event(const Event& other) = default;
        Event(Event&& other) noexcept = default;
        Event& operator=(const Event& other) = delete;
        Event& operator=(Event&& other) noexcept = delete;
        ~Event() = default;

        /**
         * @brief 事件监听器类型，属于可调用对象（函数指针、lambda 表达式等），接受 EventArg 类型的参数（建议参数名为 `e`）。
         */
        using EventListener = std::function<void(EventArg&)>;

        /**
         * @brief 订阅顺序，决定事件监听器加入委托列表的位置。
         */
        enum class SubscriptionOrder : std::uint8_t
        {
            HEAD, TAIL
        };

        /**
         * @brief 订阅事件。如果多个事件监听器订阅了同一事件，事件触发时会按订阅顺序调用。
         * 要取消订阅，请在监听器中调用上下文参数的 `unsubscribeThis` 函数。
         * @param listener 事件监听器。
         * @param order 订阅顺序，决定事件监听器加入委托列表的位置。默认为尾部。
         */
        void subscribe(EventListener listener, SubscriptionOrder order = SubscriptionOrder::TAIL);

        /**
         * @brief 触发事件。按订阅顺序调用所有事件监听器。
         */
        void trigger(EventArg& e);

        /**
         * @brief 触发事件。按订阅顺序调用所有事件监听器。
         */
        void trigger(EventArg&& e);

    private:
        std::list<EventListener> listeners;
    };

    template <typename EventArg>
    void Event<EventArg>::subscribe(EventListener listener, SubscriptionOrder order)
    {
        if(order == SubscriptionOrder::HEAD)
        {
            listeners.push_front(listener);
        }
        else
        {
            listeners.push_back(listener);
        }
    }

    template <typename EventArg>
    void Event<EventArg>::trigger(EventArg& e)
    {
        for(auto it = listeners.begin(); it != listeners.end();)
        {
            it->operator()(e);

            if(e.interrupted)
            {
                break;
            }

            if(e.unsubscribed)
            {
                it = listeners.erase(it);
            }
            else
            {
                ++it;
            }

            e.next();
        }
    }

    template <typename EventArg>
    void Event<EventArg>::trigger(EventArg&& e)     // NOLINT(cppcoreguidelines-rvalue-reference-param-not-moved)
    {
        trigger(e);
    }
}

/*
除了之前讨论的核心控制流，事件系统还有这些进阶操作：

---

## 一、监听器控制

| 操作 | 说明 | 场景 |
|------|------|------|
| **once** | 只执行一次，自动注销 | 初始化完成回调、一次性动画触发 |
| **prepend** | 插入到队列头部，优先执行 | 拦截器、中间件（如权限检查必须先跑） |
| **priority** | 按优先级排序执行 | 插件系统、多模块竞争处理权 |
| **async** | 异步/延迟执行 | 不阻塞当前流程，如下个事件循环再处理 |
| **debounce/throttle** | 防抖/节流 | 高频事件（resize、scroll、输入） |

```cpp
// 示意
signal.connect(cb);                    // 默认尾部追加
signal.prepend(cb);                    // 头部插入
signal.connect(cb, Priority::High);    // 按优先级排序
signal.once(cb);                       // 一次自动断开
signal.connect(cb).setAsync(true);     // 异步执行
```

---

## 二、事件流控制

| 操作 | 说明 | 场景 |
|------|------|------|
| **capture** | 事件到达目标前，祖先先拦截 | 全局快捷键、模态对话框拦截点击 |
| **bubble** | 事件从目标向上冒泡 | 点击子组件，父容器也感知 |
| **target only** | 仅目标处理，不冒泡 | 防止父容器误响应 |
| **redispatch** | 重新包装事件再次分发 | 跨窗口/跨线程转发 |
| **synthesize** | 伪造事件注入 | 自动化测试、录制回放 |

```cpp
// DOM 风格的三阶段
enum Phase { Capture, Target, Bubble };

node.addEventListener("click", cb, {
    .phase = Phase::Capture,  // 捕获阶段拦截
    .passive = true,          // 不调用 preventDefault（性能优化）
    .once = false
});
```

---

## 三、作用域控制

| 操作 | 说明 | 场景 |
|------|------|------|
| **suspend** | 暂停所有监听，不注销 | 模态对话框弹出时，背后界面禁用交互 |
| **resume** | 恢复暂停的监听 | 对话框关闭 |
| **mute** | 静默丢弃事件，不执行回调 | 拖拽时临时忽略 hover |
| **batch** | 批量收集，统一 flush | 状态批量更新后只触发一次重绘 |

```cpp
{
    auto guard = eventBus.suspend();  // RAII，离开作用域自动 resume
    // 这期间 emit 的事件被排队或丢弃
}
// 或
node.mute();      // 开始静默
// ... 大量更新 ...
node.unmute();    // 恢复，期间事件可选 flush 或丢弃
```

---

## 四、跨边界控制

| 操作 | 说明 | 场景 |
|------|------|------|
| **cross-thread** | 跨线程投递 | UI 线程 ←→ 工作线程 |
| **cross-process** | 跨进程 IPC 转发 | 多窗口应用、插件隔离 |
| **coalesce** | 合并同类事件 | 鼠标移动 60fps 合并为一帧一次 |
| **transform** | 事件转换映射 | 触摸 → 鼠标、滚轮 → 缩放 |

```cpp
// 跨线程：工作线程发事件，UI 线程队列消费
worker.onProgress([](int pct) {
    uiThread.dispatch([pct]{
        progressBar.setValue(pct);  // 切到 UI 线程执行
    });
});

// 合并：16ms 内多次 resize 只发一次
signal.connect([](ResizeEvent& e) {
    // 框架内部自动合并
}).setCoalesce(16ms);
```

---

## 五、调试/监控控制

| 操作 | 说明 | 场景 |
|------|------|------|
| **intercept** | 拦截所有事件，可修改或丢弃 | 调试、AOP |
| **trace** | 记录事件流路径 | 排查"谁消费了这个事件" |
| **count** | 统计触发频率 | 性能分析 |
| **inject** | 注入伪造事件 | 单元测试 |

```cpp
// 全局拦截器
eventBus.intercept([](Event& e) {
    log << "event: " << e.type << " target: " << e.target;
    if (e.type == "debug.skip") e.stopPropagation();
});
```

---

## 六、给你的框架建议

如果只需要**最小可用集**：

```
必备：connect / disconnect / once / stopPropagation
推荐：prepend / priority / suspend
可选：async / debounce / capture-bubble / cross-thread
```

如果定位**现代声明式 GUI**：

```
核心：Signal<T> + Connection RAII + 延迟队列（reentrant 安全）
进阶：事件总线（跨模块）+ 冒泡（嵌套组件）+ 合并（高频事件）
```

---

## 一句话

> **控制流的本质是"谁、在什么时候、以什么顺序、能不能被打断"——设计时先回答这四个问题，功能自然清晰。**
*/
