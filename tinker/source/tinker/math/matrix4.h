#pragma once

#include "tinker/math/quaternion.h"
#include "tinker/math/vector3.h"
#include "tinker/math/vector4.h"

#include <iostream>



// TODO: Storing data in column layout and constructor with row layout is very missleading

namespace ti::math
{
    struct Matrix4
    {
        union
        {
            Vector4 columns[4];
            struct
            {
                float m00, m10, m20, m30; // e.g. m01 = row 0, column 1
                float m01, m11, m21, m31;
                float m02, m12, m22, m32;
                float m03, m13, m23, m33;
            };
            float data[16];
        };

        Matrix4() {}
        Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w);
        Matrix4(float m00, float m10, float m20, float m30,
                float m01, float m11, float m21, float m31,
                float m02, float m12, float m22, float m32,
                float m03, float m13, float m23, float m33);

        Matrix4& Scale(const Vector3& vector);
        Matrix4& Scale(float scalar);
        Matrix4 Scaled(const Vector3& vector);
        Matrix4 Scaled(float scalar);

        Matrix4& Translate(const Vector3& vector);
        Matrix4 Translated(const Vector3& vector) const;

        Matrix4& Rotate(const Quaternion& quaternion);
        Matrix4& Rotate(float angle, const Vector3& axis);
        Matrix4& RotateX(float angle);
        Matrix4& RotateY(float angle);
        Matrix4& RotateZ(float angle);

        Matrix4& Transpose();
        Matrix4 Transposed() const;

        Matrix4& operator+=(const Matrix4& other);
        Matrix4& operator-=(const Matrix4& other);
        Matrix4& operator*=(const Matrix4& other);

        Matrix4& operator*=(float scalar);
        Matrix4& operator/=(float scalar);

        static Matrix4 RotationX(float angle);
        static Matrix4 RotationY(float angle);
        static Matrix4 RotationZ(float angle);

        static Matrix4 LookAt(const Vector3& from, const Vector3& target, const Vector3& up = Vector3::UP);

        static Matrix4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);

        static const Matrix4 IDENTITY;
    };

    std::ostream& operator<<(std::ostream& os, const Matrix4& matrix);
}