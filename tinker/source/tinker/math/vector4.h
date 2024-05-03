#pragma once



namespace ti::math
{
    struct Vector4
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
            float data[4];
        };

        Vector4() {}
        Vector4(float fillValue);
        Vector4(float x, float y, float z, float w);

        Vector4& Normalize();
        Vector4 Normalized() const;

        float Length() const;
        float LengthSquared() const;

        float Dot(const Vector4& rhs);

        Vector4& operator+=(const Vector4& other);
        Vector4& operator-=(const Vector4& other);
        Vector4& operator*=(const Vector4& other);

        Vector4& operator*=(float scalar);
        Vector4& operator/=(float scalar);

        friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
        friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
        friend Vector4 operator*(const Vector4& lhs, const Vector4& rhs);

        friend Vector4 operator-(const Vector4& vector);

        static float Dot(const Vector4& lhs, const Vector4& rhs);

        static const Vector4 ZERO;
    };
}