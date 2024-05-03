#pragma once



namespace ti::math
{
    inline constexpr float PI = 3.14159265358979323846f;
    inline constexpr float INV_PI = 1.0f / PI;

    float ToDegrees(float radians);
    float ToRadians(float degrees);
}