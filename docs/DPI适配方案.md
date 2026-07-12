# DPI 适配方案

## 背景

当前项目没有任何 DPI 感知声明，Windows 会对其进行 DPI 虚拟化（缩放位图）。对于 DirectUI 框架，必须正确适配 DPI 才能在高分屏（如 150%、200% 缩放）下获得清晰渲染。

---

## 基本原则

| 层级 | 单位 | 说明 |
|------|------|------|
| 应用层 API（用户代码） | **DIP**（Device Independent Pixel） | `Position2F`、`SizeF`、`fontSize` 等全部以 DIP 为单位 |
| D2D/DWrite 层 | **DIP** | D2D 内部自动映射到物理像素 |
| Win32 层（窗口、消息） | **物理像素** | `CreateWindowEx`、`WM_SIZE` 等是物理像素 |

**核心思路**：框架在 Win32 边界做 DIP ↔ 物理像素转换，应用层完全在 DIP 空间中工作。

---

## 改动清单

### 1. DPI 感知声明

**方式**：在 `CMakeLists.txt` 中嵌入 manifest 或调用 `SetProcessDpiAwarenessContext`。

推荐 manifest 方式（更可靠）：

新建 `GauzyUI.manifest`：

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
  <application xmlns="urn:schemas-microsoft-com:asm.v3">
    <windowsSettings>
      <dpiAware xmlns="http://schemas.microsoft.com/SMI/2005/WindowsSettings">true</dpiAware>
      <dpiAwareness xmlns="http://schemas.microsoft.com/SMI/2016/WindowsSettings">PerMonitorV2</dpiAwareness>
    </windowsSettings>
  </application>
</assembly>
```

在 `CMakeLists.txt` 中引用：

```cmake
target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/GauzyUI.manifest")
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    APP_MANIFEST "${CMAKE_SOURCE_DIR}/GauzyUI.manifest")
```

或者更简单的方案——在 `Test.cpp` / `main()` 入口处调用：

```cpp
#include <shellscalingapi.h>

// 在 main() 最开头：
SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
// 或 Win10 1607+ 用这个
// SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
```

### 2. Renderer 暴露 DPI

```cpp
// include/graphic/Renderer.hpp — 新增
public:
    [[nodiscard]] float getDpiScale() const;  // 返回 DPI 缩放比（dpi / 96.0）
    [[nodiscard]] std::pair<float, float> getDpi() const;

    void updateDpi(float dpiX, float dpiY);   // 由 Window 在 WM_DPICHANGED 时调用
```

实现：

```cpp
// src/graphic/Renderer.cpp
void Renderer::updateDpi(const float dpiX, const float dpiY)
{
    pRenderTarget->SetDpi(dpiX, dpiY);
}

float Renderer::getDpiScale() const
{
    float dpiX, dpiY;
    pRenderTarget->GetDpi(&dpiX, &dpiY);
    return dpiX / 96.0F;
}

std::pair<float, float> Renderer::getDpi() const
{
    float dpiX, dpiY;
    pRenderTarget->GetDpi(&dpiX, &dpiY);
    return { dpiX, dpiY };
}
```

> D2D `ID2D1RenderTarget::GetDpi()` 返回当前实际 DPI。`SetDpi()` 之后后续绘制自动缩放。

### 3. Window 处理 WM_DPICHANGED

在 `Window::GauzyWindowProc` 中新增：

```cpp
case WM_DPICHANGED:
{
    const float dpiX = static_cast<float>(LOWORD(wParam));
    const float dpiY = static_cast<float>(HIWORD(wParam));
    window->renderer.updateDpi(dpiX, dpiY);

    // 按系统建议尺寸重设窗口位置和大小（lParam 是 RECT*）
    const auto* rect = reinterpret_cast<RECT*>(lParam);
    SetWindowPos(hWnd, nullptr,
        rect->left, rect->top,
        rect->right - rect->left,
        rect->bottom - rect->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
    break;
}
```

### 4. 窗口创建时 DPI 换算

`Window` 构造时，用户传入的 `SizeU` 被视为 **DIP**，创建窗口前需转为物理像素：

```cpp
// src/Window.cpp — createWindow 函数
static type::WindowHandle createWindow(const std::string& title, const type::SizeU& sizeDips)
{
    // 获取系统 DPI（窗口还没创建，用系统 DPI 估算）
    const HINSTANCE hInstance = GetModuleHandle(nullptr);

    static std::once_flag gauzyWindowClassFlag;
    std::call_once(gauzyWindowClassFlag, registerWindowClass, hInstance);

    // 估算 DPI 缩放（窗口创建后 WM_DPICHANGED 会纠正）
    const UINT dpi = GetDpiForSystem();
    const float scale = static_cast<float>(dpi) / 96.0F;
    const int pixelWidth  = static_cast<int>(sizeDips.x() * scale + 0.5F);
    const int pixelHeight = static_cast<int>(sizeDips.y() * scale + 0.5F);

    const std::wstring wideTitle = util::utf8ToWide(title);
    const HWND result = CreateWindowEx(
        0, CLASS_NAME, wideTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        pixelWidth, pixelHeight,
        nullptr, nullptr, hInstance, nullptr
    );
    // ...
}
```

> 首次创建用 `GetDpiForSystem()` 估算。如果窗口创建后被拖到不同 DPI 的显示器，`WM_DPICHANGED` 会纠正。

### 5. 组件 DPI 变化通知（可选，扩展层）

有些组件需要感知 DPI 变化（如缓存了位图、计算了像素精确的布局）。可添加虚方法：

```cpp
// comp/Component.hpp
virtual void onDpiChanged(float oldScale, float newScale) {}
```

Render 流程中 `Renderer::updateDpi()` 改为通知窗口，窗口再遍历组件树：

```cpp
// Window 中
void Window::notifyDpiChanged(float oldScale, float newScale)
{
    topGroup.onDpiChanged(oldScale, newScale);
}
```

### 6. TextFormat 与字体大小

`TextFormat.fontSize` 已经是 DIP，DWrite 会根据渲染目标 DPI 自动缩放，**无需额外处理**。

### 7. 位图资源（远期）

按 DPI 加载对应分辨率：

```cpp
enum class BitmapResolution {  // 根据 DPI scale 选择
    _100,   // scale <= 1.0
    _125,   // scale <= 1.25
    _150,   // scale <= 1.5
    _200    // scale > 1.5
};

BitmapResolution resolutionForScale(float scale);
```

---

## 数据流总览

```
WM_DPICHANGED (wParam = dpi, lParam = suggested RECT)
  │
  ├─▶ Renderer::updateDpi(dpiX, dpiY)
  │     └─▶ pRenderTarget->SetDpi()
  │
  ├─▶ SetWindowPos(suggested rect)  ← 系统建议的新窗口位置和大小
  │
  ├─▶ WM_SIZE → Renderer::updateSize()  ← 触发 D2D 渲染目标 resize
  │
  └─▶ 后续 drawText / drawLine 自动以新 DPI 缩放
```

---

## 需要改动的文件汇总

| 文件 | 改动 |
|------|------|
| **新建** `GauzyUI.manifest` | DPI 感知声明 |
| `CMakeLists.txt` | 引用 manifest |
| `include/graphic/Renderer.hpp` | 新增 `getDpiScale()`、`getDpi()`、`updateDpi()` |
| `src/graphic/Renderer.cpp` | 实现上述方法 |
| `src/Window.cpp` | `WM_DPICHANGED` 处理、窗口创建时 DPI 换算 |
| `include/comp/Component.hpp` | （可选）`onDpiChanged()` 虚方法 |

---

## 与 TextFormat 的关系

`TextFormat.fontSize` 以 DIP 为单位，完全不受 DPI 变化影响——D2D/DWrite 在绘制时自动根据渲染目标的 DPI 做像素映射。例如：

- 96 DPI 下 `fontSize = 12.0F` → 12 物理像素
- 192 DPI 下 `fontSize = 12.0F` → 24 物理像素

因此 **TextFormat 无需任何 DPI 相关改动**。
