#pragma once

namespace gauzy::type
{
    template <typename T>
    struct Vec2d_T
    {
        T x;
        T y;

        Vec2d_T<T> operator+(const Vec2d_T<T>& other) const
        {
            return Vec2d_T<T>(x + other.x, y + other.y);
        }

        Vec2d_T<T> operator-(const Vec2d_T<T>& other) const
        {
            return Vec2d_T<T>(x - other.x, y - other.y);
        }

        Vec2d_T<T> operator*(float scalar) const
        {
            return Vec2d_T<T>(x * scalar, y * scalar);
        }

        Vec2d_T<T> operator/(float scalar) const
        {
            return Vec2d_T<T>(x / scalar, y / scalar);
        }

        Vec2d_T<T>& operator+=(const Vec2d_T<T>& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2d_T<T>& operator-=(const Vec2d_T<T>& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vec2d_T<T>& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vec2d_T<T>& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Vec2d_T<T>& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vec2d_T<T>& other) const
        {
            return x != other.x || y != other.y;
        }
    };

    using Vec2d_F = Vec2d_T<float>;
    using Vec2d_I = Vec2d_T<int>;
    using Vec2d = Vec2d_F;

    using Point = Vec2d_F;
    using Size = Vec2d_F;

} // namespace gauzy::type
