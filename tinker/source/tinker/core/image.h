#pragma once

#include "tinker/core/types.h"



u8* readImage(const char* filePath, u32& outWidth, u32& outHeight, u32& outChannels);
void freeImage(u8* pixels);