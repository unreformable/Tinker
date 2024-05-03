#pragma once



namespace ti::math
{
    struct Vector2
    {
        union
        {
            struct
            {
                float x;
                float y;
            };
            float data[2];
        };

        Vector2() {}
        Vector2(float fillValue);
        Vector2(float x, float y);

        Vector2& Normalize();
        Vector2 Normalized() const;

        float Length() const;
        float LengthSquared() const;

        float Dot(const Vector2& rhs);

        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);
        Vector2& operator*=(const Vector2& other);

        Vector2& operator*=(float scalar);
        Vector2& operator/=(float scalar);

        friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
        friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
        friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs);

        friend Vector2 operator-(const Vector2& vector);

        static float Dot(const Vector2& lhs, const Vector2& rhs);

        static const Vector2 RIGHT;
        static const Vector2 UP;
        static const Vector2 ZERO;
    };
}