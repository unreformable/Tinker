#include "tinker/math/vector3.h"

#include <cmath>



namespace ti::math
{
    Vector3::Vector3(float fillValue)
    : x(fillValue),
      y(fillValue),
      z(fillValue)
    {}

    Vector3::Vector3(float x, float y, float z)
    : x(x), y(y), z(z)
    {}

    Vector3& Vector3::Normalize()
    {
        *this /= Length();
        return *this;
    }

    Vector3 Vector3::Normalized() const
    {
        return Vector3(*this).Normalize();
    }

    float Vector3::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    float Vector3::LengthSquared() const
    {
        return x*x + y*y + z*z;
    }

    float Vector3::Dot(const Vector3& rhs)
    {
        return Dot(*this, rhs);
    }

    Vector3& Vector3::Cross(const Vector3& rhs)
    {
        x = (y * rhs.z) - (z * rhs.y);
        y = (z * rhs.x) - (x * rhs.z);
        z = (x * rhs.y) - (y * rhs.x);
        return *this;
    }

    Vector3 Vector3::Crossed(const Vector3& rhs) const
    {
        return Cross(*this, rhs);
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& Vector3::operator*=(const Vector3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3& Vector3::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& Vector3::operator/=(float scalar)
    {
        const float invScalar = 1.0f / scalar;
        x *= invScalar;
        y *= invScalar;
        z *= invScalar;
        return *this;
    }

    Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(lhs) += rhs;
    }

    Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(lhs) -= rhs;
    }

    Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(lhs) *= rhs;
    }

    Vector3 operator-(const Vector3& vector)
    {
        return Vector3(-vector.x, -vector.y, -vector.z);
    }

    float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
    {
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
    }

    Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(
            (lhs.y * rhs.z) - (lhs.z * rhs.y),
            (lhs.z * rhs.x) - (lhs.x * rhs.z),
            (lhs.x * rhs.y) - (lhs.y * rhs.x)
        );
    }

    const Vector3 Vector3::RIGHT   = Vector3(1,  0,  0);
    const Vector3 Vector3::UP      = Vector3(0,  1,  0);
    const Vector3 Vector3::FORWARD = Vector3(0,  0, -1);
    const Vector3 Vector3::ZERO    = Vector3(0,  0,  0);
}