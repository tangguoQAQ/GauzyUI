#include "type/BasicTypes.hpp"

namespace gauzy::type
{
    Position2F::Position2F(const D2D1_POINT_2F& d2dPoint) noexcept :
        Eigen::Vector2f{ d2dPoint.x, d2dPoint.y }
    {
    }

    Position2F& Position2F::operator=(const D2D1_POINT_2F& d2dPoint) noexcept
    {
        x() = d2dPoint.x;
        y() = d2dPoint.y;
        return *this;
    }

    Position2F::operator D2D1_POINT_2F() const
    {
        return D2D1::Point2F(x(), y());
    }

    Position2F::operator Position2U() const
    {
        return Position2U{ x(), y() };
    }



    Position2U::Position2U(const D2D1_POINT_2U& d2dPoint) noexcept :
        Eigen::Vector2<unsigned int> { d2dPoint.x, d2dPoint.y }
    {
    }

    Position2U& Position2U::operator=(const D2D1_POINT_2U& d2dPoint) noexcept
    {
        x() = d2dPoint.x;
        y() = d2dPoint.y;
        return *this;
    }
    
    Position2U::operator D2D1_POINT_2U() const
    {
        return D2D1::Point2U(x(), y());
    }

    Position2U::operator Position2F() const
    {
        return Position2F{ x(), y() };
    }



    SizeF::SizeF(const D2D1_SIZE_F& d2dSize) noexcept :
        Eigen::Vector2f{ d2dSize.width, d2dSize.height }
    {
    }

    SizeF& SizeF::operator=(const D2D1_SIZE_F& d2dSize) noexcept
    {
        x() = d2dSize.width;
        y() = d2dSize.height;
        return *this;
    }

    SizeF::operator D2D1_SIZE_F() const
    {
        return D2D1::SizeF(x(), y());
    }

    SizeF::operator SizeU() const
    {
        return SizeU{ x(), y() };
    }



    SizeU::SizeU(const D2D1_SIZE_U& d2dSize) noexcept :
        Eigen::Vector2<unsigned int> { d2dSize.width, d2dSize.height }
    {
    }

    SizeU& SizeU::operator=(const D2D1_SIZE_U& d2dSize) noexcept
    {
        x() = d2dSize.width;
        y() = d2dSize.height;
        return *this;
    }

    SizeU::operator D2D1_SIZE_U() const
    {
        return D2D1::SizeU(x(), y());
    }

    SizeU::operator SizeF() const
    {
        return SizeF{ x(), y() };
    }
} // namespace gauzy::type
