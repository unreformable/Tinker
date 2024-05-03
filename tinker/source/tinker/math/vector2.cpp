#include "tinker/math/vector2.h"

#include <cmath>



namespace ti::math
{
    Vector2::Vector2(float fillValue)
    : x(fillValue),
      y(fillValue)
    {}

    Vector2::Vector2(float x, float y)
    : x(x), y(y)
    {}

    Vector2& Vector2::Normalize()
    {
        *this /= Length();
        return *this;
    }

    Vector2 Vector2::Normalized() const
    {
        return Vector2(*this).Normalize();
    }

    float Vector2::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    float Vector2::LengthSquared() const
    {
        return x*x + y*y;
    }

    float Vector2::Dot(const Vector2& rhs)
    {
        return Dot(*this, rhs);
    }

    Vector2& Vector2::operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& Vector2::operator*=(const Vector2& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vector2& Vector2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& Vector2::operator/=(float scalar)
    {
        const float invScalar = 1.0f / scalar;
        x *= invScalar;
        y *= invScalar;
        return *this;
    }

    Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
    {
        return Vector2(lhs) += rhs;
    }

    Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
    {
        return Vector2(lhs) -= rhs;
    }

    Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
    {
        return Vector2(lhs) *= rhs;
    }

    Vector2 operator-(const Vector2& vector)
    {
        return Vector2(-vector.x, -vector.y);
    }

    float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
    {
        return lhs.x*rhs.x + lhs.y*rhs.y;
    }

    const Vector2 Vector2::RIGHT = Vector2(1, 0);
    const Vector2 Vector2::UP    = Vector2(0, 1);
    const Vector2 Vector2::ZERO  = Vector2(0, 0);
}