#pragma once

#include "tinker/core/types.h"

#include <vector>



namespace ti::core
{
    /**
     * @return The file data without null terminator.
     * @retval Empty vector if could not open file.
    */
    std::vector<u8> readFile(const char* filePath);
}