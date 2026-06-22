/**
 * @brief GauzyUI 框架基本类型的头文件。包含 `Position2F`, `SizeF` 等，封装了与 D2D 类型的桥接。
 */

#pragma once

#include <Eigen/Core>
#include <d2d1.h>

namespace gauzy::type
{
    class Position2U;

    class Position2F : public Eigen::Vector2f
    {
    public:
        using Eigen::Vector2f::Vector2f;

        explicit Position2F(const D2D1_POINT_2F& d2dPoint) noexcept;
        Position2F& operator=(const D2D1_POINT_2F& d2dPoint) noexcept;

        explicit operator D2D1_POINT_2F() const;

        explicit operator Position2U() const;
    };

    class Position2U : public Eigen::Vector2<unsigned int>
    {
    public:
        using Eigen::Vector2<unsigned int>::Vector2;

        explicit Position2U(const D2D1_POINT_2U& d2dPoint) noexcept;
        Position2U& operator=(const D2D1_POINT_2U& d2dPoint) noexcept;

        explicit operator D2D1_POINT_2U() const;

        explicit operator Position2F() const;
    };

    class SizeU;

    class SizeF : public Eigen::Vector2f
    {
    public:
        using Eigen::Vector2f::Vector2f;

        explicit SizeF(const D2D1_SIZE_F& d2dSize) noexcept;
        SizeF& operator=(const D2D1_SIZE_F& d2dSize) noexcept;

        explicit operator D2D1_SIZE_F() const;

        explicit operator SizeU() const;
    };

    class SizeU : public Eigen::Vector2<unsigned int>
    {
    public:
        using Eigen::Vector2<unsigned int>::Vector2;

        explicit SizeU(const D2D1_SIZE_U& d2dSize) noexcept;
        SizeU& operator=(const D2D1_SIZE_U& d2dSize) noexcept;

        explicit operator D2D1_SIZE_U() const;

        explicit operator SizeF() const;
    };

} // namespace gauzy::type
