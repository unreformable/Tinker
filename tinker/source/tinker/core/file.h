#pragma once

#include "tinker/core/types.h"



namespace ti::core
{
    void readFile(const char *filePath, u8 *buffer, size_t *fileSize);

    u8* readFile(const char *filePath, size_t *fileSize);
}