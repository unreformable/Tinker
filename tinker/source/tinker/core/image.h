#pragma once

#include "tinker/core/types.h"



namespace ti::core
{
    u8* readImage(const char *filePath, u32 *width, u32 *height, u8 *channels);
    void freeImage(u8 *pixels);
}