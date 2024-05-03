#include "tinker/core/image.h"

#include "tinker/core/debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"



u8* readImage(const char* filePath, u32& outWidth, u32& outHeight, u32& outChannels)
{
    int w, h, c;
    u8* pixels = stbi_load(filePath, &w, &h, &c, 0);

    TI_REQUIRE(pixels, "Pixels are valid");
    TI_REQUIRE(w > 0, "Width is greater than 0");
    TI_REQUIRE(h > 0, "Height is greater than 0");
    TI_REQUIRE(c > 0, "Channels count is greater than 0");

    outWidth = (u32)w;
    outHeight = (u32)h;
    outChannels = (u32)c;
    return pixels;
}

void freeImage(u8* pixels)
{
    stbi_image_free(pixels);
}