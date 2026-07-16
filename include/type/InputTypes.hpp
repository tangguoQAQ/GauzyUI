#pragma once

#include <cstdint>

#include "Windows.h"

namespace gauzy::type
{
    enum class MouseButtonType : std::uint8_t
    {
        Left,
        Right,
        /// @brief 滚轮中键
        Middle,
        /// @brief 侧键-后退
        X1,
        /// @brief 侧键-前进
        X2
    };

    [[nodiscard]] MouseButtonType toMouseButtonType(UINT uMsg, WPARAM wParam) noexcept;
}