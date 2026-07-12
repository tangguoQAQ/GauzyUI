# TextFormat 组件设计与实现

## 概述

`TextFormat` 是对 DirectWrite `IDWriteTextFormat` 的属性描述器（value type），遵循 `Brush` 模式——只存属性、不持有 D2D/DWrite 资源，资源由 `Renderer` 按需创建并缓存。

---

## 设计方案

### 方案对比

| 方案 | 描述 | 权衡 |
|------|------|------|
| **A. Value type + Renderer 缓存（选定）** | 存属性 + hash/==，Renderer 内部 `map` 缓存 `IDWriteTextFormat` | 与 Brush 一致，值语义，易于拷贝传递 |
| B. TextFormat 直接持有 `IDWriteTextFormat` | 构造时需传入 `IDWriteFactory` | 耦合 Renderer，拷贝需 AddRef，较重 |
| C. 无缓存、每次 drawText 创建 | 最简洁但性能差 | 频繁创建 DWrite 对象 |

**选定方案 A**，与 `Brush`/`SolidColorBrush` 模式一致。

---

## 文件清单

### 新建文件

| 文件 | 说明 |
|------|------|
| `include/graphic/TextFormat.hpp` | TextFormat 结构体声明 |
| `src/graphic/TextFormat.cpp` | TextFormat 实现（==、!=、hash） |

### 修改文件

| 文件 | 变更 |
|------|------|
| `include/graphic/Renderer.hpp` | 加 `#include "TextFormat.hpp"`、缓存成员、辅助方法 |
| `src/graphic/Renderer.cpp` | 实现 `drawText` 和 `getOrCreateTextFormat` |
| `CMakeLists.txt` | `src/graphic/TextFormat.cpp` 加入 CPP_FILES |

---

## 接口设计

### `include/graphic/TextFormat.hpp`

```cpp
#pragma once

#include <cstddef>
#include <string>

#include <dwrite.h>

namespace gauzy::graphic
{
    struct TextFormat
    {
        // ── 字体属性 ──────────────────────────────────────
        std::string fontFamily{ "Segoe UI" };
        float fontSize{ 12.0F };
        DWRITE_FONT_WEIGHT fontWeight{ DWRITE_FONT_WEIGHT_NORMAL };
        DWRITE_FONT_STYLE fontStyle{ DWRITE_FONT_STYLE_NORMAL };
        DWRITE_FONT_STRETCH fontStretch{ DWRITE_FONT_STRETCH_NORMAL };

        // ── 布局属性 ──────────────────────────────────────
        DWRITE_TEXT_ALIGNMENT textAlignment{ DWRITE_TEXT_ALIGNMENT_LEADING };
        DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment{ DWRITE_PARAGRAPH_ALIGNMENT_NEAR };
        DWRITE_WORD_WRAPPING wordWrapping{ DWRITE_WORD_WRAPPING_WRAP };

        // ── 区域 ──────────────────────────────────────────
        std::string localeName{ "zh-CN" };

        // Rule-of-five
        TextFormat() = default;
        TextFormat(const TextFormat&) = default;
        TextFormat(TextFormat&&) noexcept = default;
        TextFormat& operator=(const TextFormat&) = default;
        TextFormat& operator=(TextFormat&&) noexcept = default;
        ~TextFormat() = default;

        bool operator==(const TextFormat& rhs) const noexcept;
        bool operator!=(const TextFormat& rhs) const noexcept;

        [[nodiscard]] std::size_t hash() const noexcept;
    };
} // namespace gauzy::graphic
```

### `include/graphic/Renderer.hpp` — 改动

```cpp
// ── 新增 include ─────────────────────────────────────────
#include "graphic/TextFormat.hpp"

// ── private 成员新增 ─────────────────────────────────────
mutable std::unordered_map<std::size_t, wil::com_ptr<IDWriteTextFormat>> textFormatCache;

// ── private 方法新增 ─────────────────────────────────────
[[nodiscard]] IDWriteTextFormat& getOrCreateTextFormat(const TextFormat& textFormat) const;
```

### `Renderer::drawText` 绘制流程

```
drawText(pos, text, brush, textFormat)
  │
  ├─ hash = textFormat.hash()
  ├─ textFormatCache 命中? ──→ 直接复用 IDWriteTextFormat
  │       │
  │       └─ 未命中: CreateTextFormat(fontFamily, fontSize, ...) → 缓存
  │
  ├─ utf8ToWide(text)
  ├─ CreateTextLayout(wideText, textFormat, renderTargetWidth, renderTargetHeight)
  └─ DrawTextLayout(position, textLayout, brush)
```

---

## 实现细节

### `src/graphic/TextFormat.cpp`

```cpp
#include "graphic/TextFormat.hpp"

#include <functional>

namespace gauzy::graphic
{
    bool TextFormat::operator==(const TextFormat& rhs) const noexcept
    {
        return fontFamily == rhs.fontFamily
            && fontSize == rhs.fontSize
            && fontWeight == rhs.fontWeight
            && fontStyle == rhs.fontStyle
            && fontStretch == rhs.fontStretch
            && textAlignment == rhs.textAlignment
            && paragraphAlignment == rhs.paragraphAlignment
            && wordWrapping == rhs.wordWrapping
            && localeName == rhs.localeName;
    }

    bool TextFormat::operator!=(const TextFormat& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    std::size_t TextFormat::hash() const noexcept
    {
        std::size_t seed = 0x3A4B5C6D;

        auto combine = [&seed](std::size_t value, std::size_t constant) {
            seed ^= (seed << 6) + (seed >> 2) + constant + value;
        };

        combine(std::hash<std::string>{}(fontFamily),          0xA1B2C3D4);
        combine(static_cast<std::size_t>(fontSize * 100.0F),   0xE5F6A7B8);
        combine(static_cast<std::size_t>(fontWeight),          0xC9D0E1F2);
        combine(static_cast<std::size_t>(fontStyle),           0x3A4B5C6D);
        combine(static_cast<std::size_t>(fontStretch),         0x7E8F9A0B);
        combine(static_cast<std::size_t>(textAlignment),       0xC1D2E3F4);
        combine(static_cast<std::size_t>(paragraphAlignment),  0xA5B6C7D8);
        combine(static_cast<std::size_t>(wordWrapping),        0xE9F0A1B2);
        combine(std::hash<std::string>{}(localeName),          0xC3D4E5F6);

        return seed;
    }
} // namespace gauzy::graphic
```

### Renderer 新增实现

```cpp
// ── 需要追加的 include ──────────────────────────────────
#include "Util.hpp"
#include "graphic/TextFormat.hpp"

// ── getOrCreateTextFormat ────────────────────────────────
IDWriteTextFormat& Renderer::getOrCreateTextFormat(const TextFormat& textFormat) const
{
    const std::size_t hash = textFormat.hash();

    if (const auto it = textFormatCache.find(hash);
        it != textFormatCache.end())
    {
        return *it->second;
    }

    const std::wstring wideFamily = util::utf8ToWide(textFormat.fontFamily);
    const std::wstring wideLocale = util::utf8ToWide(textFormat.localeName);

    wil::com_ptr<IDWriteTextFormat> pFormat;
    THROW_IF_FAILED(pDWriteFactory->CreateTextFormat(
        wideFamily.c_str(),
        nullptr,                         // fontCollection (null = system collection)
        textFormat.fontWeight,
        textFormat.fontStyle,
        textFormat.fontStretch,
        textFormat.fontSize,
        wideLocale.c_str(),
        &pFormat));

    THROW_IF_FAILED(pFormat->SetTextAlignment(textFormat.textAlignment));
    THROW_IF_FAILED(pFormat->SetParagraphAlignment(textFormat.paragraphAlignment));
    THROW_IF_FAILED(pFormat->SetWordWrapping(textFormat.wordWrapping));

    auto& ref = *pFormat;
    textFormatCache[hash] = std::move(pFormat);
    return ref;
}

// ── drawText ─────────────────────────────────────────────
void Renderer::drawText(const type::Position2F& position, const std::string& text,
    const Brush& brush, const TextFormat& textFormat) const
{
    auto& dwriteFormat = getOrCreateTextFormat(textFormat);
    const auto pBrush = brushMap.at(brush.hash());

    const std::wstring wideText = util::utf8ToWide(text);

    wil::com_ptr<IDWriteTextLayout> pTextLayout;
    const auto maxSize = static_cast<D2D1_SIZE_F>(getSize());

    THROW_IF_FAILED(pDWriteFactory->CreateTextLayout(
        wideText.c_str(),
        static_cast<UINT32>(wideText.length()),
        &dwriteFormat,
        maxSize.width,
        maxSize.height,
        &pTextLayout));

    pRenderTarget->DrawTextLayout(
        static_cast<D2D1_POINT_2F>(position),
        pTextLayout.get(),
        pBrush.get());
}
```

### `CMakeLists.txt` 改动

在 CPP_FILES 中 `Renderer.cpp` 行后追加：

```cmake
    "${CMAKE_SOURCE_DIR}/src/graphic/Renderer.cpp"
    "${CMAKE_SOURCE_DIR}/src/graphic/TextFormat.cpp"    # <-- 新增
```

`TextFormat.hpp` 已在 H_FILES 第 33 行，无需改动。

---

## 使用示例

```cpp
// 创建 TextFormat
gauzy::graphic::TextFormat titleFormat;
titleFormat.fontFamily = "Microsoft YaHei";
titleFormat.fontSize = 24.0F;
titleFormat.fontWeight = DWRITE_FONT_WEIGHT_BOLD;
titleFormat.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;

// 注册画刷
auto whiteBrush = renderer.registerSolidColorBrush(
    gauzy::graphic::SolidColorBrush{ gauzy::type::Color{ 0xFF, 0xFF, 0xFF } });

// 绘制文字
renderer.drawText({ 100.0F, 50.0F }, "Hello GauzyUI", whiteBrush, titleFormat);
```

---

## 属性参考

| 属性 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `fontFamily` | `std::string` | `"Segoe UI"` | 字体名称 |
| `fontSize` | `float` | `12.0F` | 字号（DIP） |
| `fontWeight` | `DWRITE_FONT_WEIGHT` | `NORMAL` | 字重 |
| `fontStyle` | `DWRITE_FONT_STYLE` | `NORMAL` | 字形 |
| `fontStretch` | `DWRITE_FONT_STRETCH` | `NORMAL` | 字宽 |
| `textAlignment` | `DWRITE_TEXT_ALIGNMENT` | `LEADING` | 水平对齐 |
| `paragraphAlignment` | `DWRITE_PARAGRAPH_ALIGNMENT` | `NEAR` | 垂直对齐 |
| `wordWrapping` | `DWRITE_WORD_WRAPPING` | `WRAP` | 换行策略 |
| `localeName` | `std::string` | `"zh-CN"` | 区域（影响字体回退） |

---

## 后续可扩展方向

- 支持自定义 `FontCollection`（替换 `CreateTextFormat` 的 `nullptr` 参数）
- `TextLayout` 返回值（支持命中测试、度量等）
- 为 `drawText` 增加 `maxWidth` / `maxHeight` 参数（当前使用渲染目标尺寸）
- 全局默认 `TextFormat` 单例
