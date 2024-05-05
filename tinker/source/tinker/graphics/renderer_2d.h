#pragma once

#include "tinker/math/vector2.h"
#include "tinker/math/vector4.h"



namespace ti::graphics
{
    void renderQuad(math::Vector2 pos, math::Vector2 size, float rotation, math::Vector3 color);
    void renderCircle(math::Vector2 pos, float radius, math::Vector3 color);
}