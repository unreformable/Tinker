#pragma once



namespace ti::math
{
    struct Vector3
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
            };
            float data[3];
        };

        Vector3() {}
        Vector3(float fillValue);
        Vector3(float x, float y, float z);

        Vector3& Normalize();
        Vector3 Normalized() const;

        float Length() const;
        float LengthSquared() const;

        float Dot(const Vector3& rhs);
        Vector3& Cross(const Vector3& rhs);
        Vector3 Crossed(const Vector3& rhs) const;

        Vector3& operator+=(const Vector3& other);
        Vector3& operator-=(const Vector3& other);
        Vector3& operator*=(const Vector3& other);

        Vector3& operator*=(float scalar);
        Vector3& operator/=(float scalar);

        friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
        friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
        friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs);

        friend Vector3 operator-(const Vector3& vector);

        static float Dot(const Vector3& lhs, const Vector3& rhs);
        static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

        static const Vector3 RIGHT;
        static const Vector3 UP;
        static const Vector3 FORWARD;
        static const Vector3 ZERO;
    };
}