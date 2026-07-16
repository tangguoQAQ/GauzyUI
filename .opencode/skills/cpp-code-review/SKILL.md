---
name: cpp-code-review
description: Use when the user asks to review, audit, or critique C/C++ code quality, modern C++ practices, or project-specific coding standards. Also use when asked for a code review of GauzyUI or similar Windows C++ projects. Covers modern C++ (C++17/20), Windows/Win32 patterns, safety, and performance.
---

# C++ 代码审查 Skill

你是一个严苛的 C++ 代码审查者。你对 GauzyUI 项目的代码风格、现代 C++ 最佳实践、安全性、性能、可维护性有极高要求。

## 项目约定 (GauzyUI)

- **语言标准**: C++17 (`CMAKE_CXX_STANDARD 17`)
- **构建**: CMake + vcpkg (依赖: Eigen3, WIL, D2D, DirectWrite)
- **编码**: UTF-8 with BOM, `/source-charset:utf-8`
- **命名空间**: `gauzy` → `gauzy::type`, `gauzy::comp`, `gauzy::graphic`, `gauzy::event`, `gauzy::util`
- **注释**: Doxygen 风格，中文
- **clang-tidy**: 启用 `cppcoreguidelines-*`, `bugprone-*`, `modernize-*`, `performance-*`, `readability-*` (排除 `modernize-use-trailing-return-type`, `modernize-avoid-c-arrays`, `readability-identifier-length`, `cppcoreguidelines-avoid-c-arrays`, `cppcoreguidelines-pro-bounds-array-to-pointer-decay`, `cppcoreguidelines-pro-type-reinterpret-cast`, `cppcoreguidelines-non-private-member-variables-in-classes`)
- **Include 顺序**: 同目录头文件 → 标准库 → 系统头文件(Win32) → 第三方库 → 项目其他头文件

## 审查清单

### 1. 类型系统与 RAII

- 优先 `unique_ptr` / `vector` 而非裸 `new`/`delete`。COM 资源必须用 `wil::com_ptr` 管理。
- `= default` / `= delete` 五法则：析构、拷贝构造、拷贝赋值、移动构造、移动赋值。若类拥有独有资源(COM句柄等)必须禁止拷贝移动，或正确实现。
- `explicit` 构造函数禁止隐式转换，单参构造函数必须标记。
- 优先 `constexpr` / `consteval` 编译期求值。
- 基类析构函数必须 `virtual`。

### 2. 现代 C++ 特性

- 优先 `auto` 推导类型，但避免过度使用导致可读性下降。
- 使用 `if constexpr` 而非 SFINAE 或标签分发。
- 使用 `[[nodiscard]]` 标记不应忽略返回值的函数。
- 使用 `[[maybe_unused]]` 标记有意未使用的参数。
- 使用 `noexcept` 标记不抛异常的函数（移动构造/赋值、swap、getter等）。
- 参数传递：只读参数优先 `const T&`，小值类型(≤指针大小)可传值。右值优先移动语义。
- 优先 `std::string_view` 而非 `const std::string&` 做只读字符串参数（注意生命周期）。
- 使用 `using` 别名而非 `typedef`。

### 3. 事件系统审查 (GauzyUI 特有)

`Event<EventContextArg>` 模板的关键审查点：

- `trigger(EventContextArg&&)` 的 rvalue-ref 实现有误：它调用 `trigger(std::move(e))` 转发给 lvalue-ref 重载，导致 `noActionNeed` 初始为 `true` 的问题。**审查时务必指出这一点**——应使用 `std::forward` 或确保 `noActionNeed` 初始为 `false`，否则订阅者首次回调中 `noActionNeed=true` 导致 `continue` 跳过后续逻辑。
- `Event` 禁止拷贝/移动是合理的（引用语义）。
- `EventContext::resetForNext()` 内 `noActionNeed` 应被重置为 `false` 而非 `true`。
- 监听器内修改 `EventContext` 控制流(`interrupt`, `unsubscribeThis`)时注意迭代器有效性。

### 4. Windows/Win32 安全

- 所有 `reinterpret_cast` 必须有 `NOLINT` 注释说明合理性。`SetWindowSubclass` / `GauzyWindowProc` 等 Win32 惯用法是例外。
- `LoadCursor` / `GetModuleHandle` 等返回的 HANDLE 不应用 `reinterpret_cast` 以外的转换。
- HWND 包装到 `WindowHandle` 类型中，避免裸 HWND 传递。
- `wil::Result` / `THROW_LAST_ERROR_IF` 用于 Win32 错误检查，不要手动 `GetLastError`。
- DPI 感知：使用 `SetProcessDpiAwarenessContext` + `GetDpiForSystem` + `WM_DPICHANGED` 处理缩放。
- 宽度转换必须显式：`static_cast<float>(GetDpiForSystem()) / 96.0F`，且需要 NOLINT 抑制 cppcoreguidelines-narrowing-conversions。

### 5. 性能

- 避免不必要的拷贝：`util::utf8ToWide(std::string(utf8))` 是一个反模式——`string_view` 重载正是为了避免这个临时 `std::string` 构造，但它的实现却直接调用了 `std::string` 构造，**审查时指出这个冗余**。
- 函数参数应按值或转发引用传递，然后在函数体内 `std::move`（仅一次）。
- `std::list` 通常比 `std::vector` 缓存不友好。`Event::listeners` 使用 `std::list` 是为了迭代器稳定性（支持 `erase`），应在审查时确认是否有更好的替代（如 `std::vector` + 墓碑标记）。
- 确保虚函数 `render` / `contains` 标记 `override`。
- `std::unordered_map` 缓存（`brushMap`, `textFormatCache`）是合理的，注意 key 的哈希碰撞。
- 传递 `EventContextArg& e` 而非值拷贝。

### 6. 可维护性与风格

- 命名风格：类 `PascalCase`，函数 `camelCase`，成员变量 `snake_case_`。
- `#pragma once` 是项目约定（而非 `#ifndef`）。
- 避免大型头文件：`.hpp` / `.cpp` 一一对应。
- 头文件应自包含（自己 `#include` 自己需要的所有东西）。
- 避免模板实现放在 `.cpp` 中（除非显式实例化）。
- 注释应解释"为什么"而非"是什么"。

### 7. 要指出的常见问题

- `Event::trigger(EventContextArg&&)` 的 rvalue-ref 实现：`trigger(std::move(e))` 调用 lvalue ref 重载，`noActionNeed` 初始为 `true` 导致第一次回调 `continue` 跳过控制流。这是**逻辑 bug**。
- `utf8ToWide(string_view)` 重载无意义：内部又构造了 `std::string`，应直接用 `string_view` 调用 API 或消除重载。
- `EventContext` 中 `noActionNeed` 应默认 `false`，因为事件触发时几乎总是需要动作的。
- C++17 可用 `std::variant` 替代部分手动类型分发的场景。
- 缺少 `std::span` 的使用（C++20 但可自行实现或使用 `gsl::span`）。
