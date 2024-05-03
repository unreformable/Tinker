#pragma once



namespace ti::math
{
    struct Quaternion
    {
        float w;
        float x;
        float y;
        float z;

        Quaternion() {}
        Quaternion(float w, float x, float y, float z);

        Quaternion& Normalize();
        Quaternion Normalized() const;

        float Length() const;
        float LengthSquared() const;

        Quaternion& Inverse();
        Quaternion Inversed() const;

        Quaternion& operator+=(const Quaternion& other);
        Quaternion& operator-=(const Quaternion& other);
        Quaternion& operator*=(const Quaternion& other);

        Quaternion& operator*=(float scalar);
        Quaternion& operator/=(float scalar);
    };
}