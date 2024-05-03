#include "tinker/core/file.h"

#include "tinker/core/debug.h"

#include <fstream>



namespace ti::core
{
    std::vector<u8> readFile(const char* filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if(!file.is_open())
        {
            TI_ERROR("Could not open file %s", filePath);
            return std::vector<u8>();
        }

        const size_t fileSize = (size_t)file.tellg();
        file.seekg(0);

        std::vector<u8> buffer(fileSize);
        file.read((char*)buffer.data(), fileSize);

        return buffer;
    }
}