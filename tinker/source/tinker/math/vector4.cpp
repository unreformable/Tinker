#include "tinker/math/vector4.h"

#include <cmath>



namespace ti::math
{
    Vector4::Vector4(float fillValue)
    : x(fillValue),
      y(fillValue),
      z(fillValue),
      w(fillValue)
    {}

    Vector4::Vector4(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w)
    {}

    Vector4& Vector4::Normalize()
    {
        *this /= Length();
        return *this;
    }

    Vector4 Vector4::Normalized() const
    {
        return Vector4(*this).Normalize();
    }

    float Vector4::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    float Vector4::LengthSquared() const
    {
        return x*x + y*y + z*z + w*w;
    }

    float Vector4::Dot(const Vector4& rhs)
    {
        return Dot(*this, rhs);
    }

    Vector4& Vector4::operator+=(const Vector4& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    Vector4& Vector4::operator-=(const Vector4& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    Vector4& Vector4::operator*=(const Vector4& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    Vector4& Vector4::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Vector4& Vector4::operator/=(float scalar)
    {
        const float invScalar = 1.0f / scalar;
        x *= invScalar;
        y *= invScalar;
        z *= invScalar;
        w *= invScalar;
        return *this;
    }

    Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs) += rhs;
    }

    Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs) -= rhs;
    }

    Vector4 operator*(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs) *= rhs;
    }

    Vector4 operator-(const Vector4& vector)
    {
        return Vector4(-vector.x, -vector.y, -vector.z, -vector.w);
    }

    float Vector4::Dot(const Vector4& lhs, const Vector4& rhs)
    {
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
    }

    const Vector4 Vector4::ZERO = Vector4(0, 0, 0, 0);
}