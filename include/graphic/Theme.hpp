#pragma once

#include "type/Color.hpp"
#include "graphic/Brush.hpp"
#include "graphic/TextFormat.hpp"

namespace gauzy::graphic
{
    /**
     * @brief 主题类，决定一个渲染器上组件的视觉样式，如背景颜色、字体格式等等。
     * @details 一个渲染器的主题对象将储存其所属组件的默认绘图资源，
     */
    struct Theme
    {
        type::Color backgroundColor;
        type::Color foregroundColor;

        SolidColorBrush backgroundBrush;
        SolidColorBrush foregroundBrush;

        TextFormat bodyTextFormat;
    };

    const TextFormat GAUZY_BODY_TEXT_FORMAT{ "Noto Sans SC", 14.0F, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, "zh-CN" };
    
    const type::Color GAUZY_LIGHT_THEME_BACKGROUND_COLOR{ 0xFFFFFF, 1.0F };
    const type::Color GAUZY_LIGHT_THEME_FOREGROUND_COLOR{ 0x000000, 0.85F };
    const Theme GAUZY_LIGHT_THEME{
        GAUZY_LIGHT_THEME_BACKGROUND_COLOR,
        GAUZY_LIGHT_THEME_FOREGROUND_COLOR,
        SolidColorBrush{ GAUZY_LIGHT_THEME_BACKGROUND_COLOR },
        SolidColorBrush{ GAUZY_LIGHT_THEME_FOREGROUND_COLOR  },
        GAUZY_BODY_TEXT_FORMAT
    };

    const type::Color GAUZY_DARK_THEME_BACKGROUND_COLOR{ 0x1E1E1E, 1.0F };
    const type::Color GAUZY_DARK_THEME_FOREGROUND_COLOR{ 0xFFFFFF, 0.85F };
    const Theme GAUZY_DARK_THEME{
        GAUZY_DARK_THEME_BACKGROUND_COLOR,
        GAUZY_DARK_THEME_FOREGROUND_COLOR,
        SolidColorBrush{ GAUZY_DARK_THEME_BACKGROUND_COLOR },
        SolidColorBrush{ GAUZY_DARK_THEME_FOREGROUND_COLOR },
        GAUZY_BODY_TEXT_FORMAT
    };
}