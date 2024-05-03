#include "tinker/math/trigonometry.h"



namespace ti::math
{
    float ToDegrees(float radians)
    {
        return radians * (INV_PI * 180.0f);
    }

    float ToRadians(float degrees)
    {
        return degrees * (PI / 180.0f);
    }
}