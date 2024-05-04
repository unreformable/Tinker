#include "tinker/core/image.h"

#include "tinker/core/debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"



namespace ti::core
{
    u8* readImage(const char *filePath, u32 *width, u32 *height, u8 *channels)
    {
        int w, h, c;
        u8* pixels = stbi_load(filePath, &w, &h, &c, 0);
        if(!pixels)
        {
            *width = 0;
            *height = 0;
            *channels = 0;
            return NULL;
        }

        *width = (u32)w;
        *height = (u32)h;
        *channels = (u8)c;
        return pixels;
    }

    void freeImage(u8 *pixels)
    {
        stbi_image_free(pixels);
    }
}