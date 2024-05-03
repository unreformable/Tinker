#include "tinker/math/matrix4.h"

#include "tinker/core/debug.h"

#include <cmath>
#include <iomanip>



// TODO: How to deal with APIs projection matrix difference? Create different projection matrix depending on API?
//       (NDC depth is [-1, 1] or [0, 1] depending on API, z-axis is different direction depending on API)

namespace ti::math
{
    Matrix4::Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w)
    : columns{ x, y, z, w }
    {}

    Matrix4::Matrix4(float m00, float m10, float m20, float m30,
                     float m01, float m11, float m21, float m31,
                     float m02, float m12, float m22, float m32,
                     float m03, float m13, float m23, float m33)
    : data{ m00, m10, m20, m30,
            m01, m11, m21, m31,
            m02, m12, m22, m32,
            m03, m13, m23, m33 }
    {}

    Matrix4& Matrix4::Scale(const Vector3& vector)
    {
        data[0]  *= vector.x;
        data[5]  *= vector.y;
        data[10] *= vector.z;
        return *this;
    }

    Matrix4& Matrix4::Scale(float scalar)
    {
        data[0]  *= scalar;
        data[5]  *= scalar;
        data[10] *= scalar;
        return *this;
    }

    Matrix4 Matrix4::Scaled(const Vector3& vector)
    {
        return Matrix4(*this).Scale(vector);
    }

    Matrix4 Matrix4::Scaled(float scalar)
    {
        return Matrix4(*this).Scale(scalar);
    }

    /**
     * @brief For explanation look column-major order multiplication here:
     *        https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/row-major-vs-column-major-vector.html
    */
    Matrix4& Matrix4::Translate(const Vector3& vector)
    {
        columns[3] += (columns[0] * vector.x) + (columns[1] * vector.y) + (columns[2] * vector.z);
        return *this;
    }

    Matrix4 Matrix4::Translated(const Vector3& vector) const
    {
        return Matrix4(*this).Translate(vector);
    }

    Matrix4& Matrix4::Rotate(const Quaternion& quaternion) { TI_ERROR("I am not implemented and delete #include \"core/debug.h\""); return *this; }
    Matrix4& Matrix4::Rotate(float angle, const Vector3& axis) { TI_ERROR("I am not implemented and delete #include \"core/debug.h\""); return *this; }

    Matrix4& Matrix4::RotateX(float angle)
    {
        *this *= Matrix4::RotationX(angle);
        return *this;
    }

    Matrix4& Matrix4::RotateY(float angle)
    {
        *this *= Matrix4::RotationY(angle);
        return *this;
    }

    Matrix4& Matrix4::RotateZ(float angle)
    {
        *this *= Matrix4::RotationZ(angle);
        return *this;
    }

    Matrix4& Matrix4::Transpose()
    {
        float tmp;
        tmp = m01; m01 = m10; m10 = tmp;
        tmp = m02; m02 = m20; m20 = tmp;
        tmp = m03; m03 = m30; m30 = tmp;
        tmp = m12; m12 = m21; m21 = tmp;
        tmp = m13; m13 = m31; m31 = tmp;
        tmp = m23; m23 = m32; m32 = tmp;
        return *this;
    }

    Matrix4 Matrix4::Transposed() const
    {
        return Matrix4{
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33
        };
    }

    Matrix4& Matrix4::operator+=(const Matrix4& other)
    {
        for(int i = 0; i < 16; i++)
            data[i] += other.data[i];
        return *this;
    }

    Matrix4& Matrix4::operator-=(const Matrix4& other)
    {
        for(int i = 0; i < 16; i++)
            data[i] -= other.data[i];
        return *this;
    }

    Matrix4& Matrix4::operator*=(const Matrix4& other)
    {
        const Matrix4 transp = Transposed();

        columns[0] = Vector4(
            Vector4::Dot(transp.columns[0], other.columns[0]),
            Vector4::Dot(transp.columns[1], other.columns[0]),
            Vector4::Dot(transp.columns[2], other.columns[0]),
            Vector4::Dot(transp.columns[3], other.columns[0]));
        
        columns[1] = Vector4(
            Vector4::Dot(transp.columns[0], other.columns[1]),
            Vector4::Dot(transp.columns[1], other.columns[1]),
            Vector4::Dot(transp.columns[2], other.columns[1]),
            Vector4::Dot(transp.columns[3], other.columns[1]));
        
        columns[2] = Vector4(
            Vector4::Dot(transp.columns[0], other.columns[2]),
            Vector4::Dot(transp.columns[1], other.columns[2]),
            Vector4::Dot(transp.columns[2], other.columns[2]),
            Vector4::Dot(transp.columns[3], other.columns[2]));
        
        columns[3] = Vector4(
            Vector4::Dot(transp.columns[0], other.columns[3]),
            Vector4::Dot(transp.columns[1], other.columns[3]),
            Vector4::Dot(transp.columns[2], other.columns[3]),
            Vector4::Dot(transp.columns[3], other.columns[3]));

        return *this;
    }

    Matrix4& Matrix4::operator*=(float scalar)
    {
        for(int i = 0; i < 16; i++)
            data[i] *= scalar;
        return *this;
    }

    Matrix4& Matrix4::operator/=(float scalar)
    {
        const float invScalar = 1.0f / scalar;
        for(int i = 0; i < 16; i++)
            data[i] *= invScalar;
        return *this;
    }

    Matrix4 Matrix4::RotationX(float angle)
    {
        const float s = std::sin(angle);
        const float c = std::cos(angle);

        return Matrix4{
            1,  0, 0, 0,
            0,  c, s, 0,
            0, -s, c, 0,
            0,  0, 0, 1
        };
    }

    Matrix4 Matrix4::RotationY(float angle)
    {
        const float s = std::sin(angle);
        const float c = std::cos(angle);

        return Matrix4{
            c, 0, -s, 0,
            0, 1,  0, 0,
            s, 0,  c, 0,
            0, 0,  0, 1
        };
    }

    Matrix4 Matrix4::RotationZ(float angle)
    {
        const float s = std::sin(angle);
        const float c = std::cos(angle);

        return Matrix4{
             c, s, 0, 0,
            -s, c, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1
        };
    }

    Matrix4 Matrix4::LookAt(const Vector3& from, const Vector3& target, const Vector3& up)
    {
        const Vector3 f = (target - from).Normalized();
        const Vector3 r = Vector3::Cross(f, up);
        const Vector3 u = Vector3::Cross(r, f);

        return Matrix4{
            r.x,                        u.x,                         - f.x,                    0,
            r.y,                        u.y,                         - f.y,                    0,
            r.z,                        u.z,                         - f.z,                    0,
            - Vector3::Dot(r, from),    - Vector3::Dot(u, from),     Vector3::Dot(f, from),    1
        };
    }

    Matrix4 Matrix4::Perspective(float fov, float aspect, float near, float far)
    {
        const float invTan = 1.0f / std::tan(fov / 2.0f);
        const float depth = far - near;

        return Matrix4{
            invTan/aspect,    0,         0,                           0,
            0,                invTan,    0,                           0,
            0,                0,         - (far + near)/depth,        - 1,
            0,                0,         - (2 * far * near)/depth,    0
        };
    }

    Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        const float height = (top - bottom);
        const float width = (right - left);
        const float depth = (far - near);

        return Matrix4{
            2/width,                   0,                          0,                       0,
            0,                         2/height,                   0,                       0,
            0,                         0,                          - 2/depth,               0,
            - (right + left)/width,    - (top + bottom)/height,    - (far + near)/depth,    1
        };
    }

    const Matrix4 Matrix4::IDENTITY(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    std::ostream& operator<<(std::ostream& os, const Matrix4& matrix)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                os << matrix.data[i*4 + j] << ' ';
            }
            os << '\n';
        }
        return os;
    }
}