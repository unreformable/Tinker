#include "tinker/core/file.h"

#include "tinker/core/debug.h"



namespace ti::core
{
    void readFile(const char *filePath, u8 *buffer, size_t *fileSize)
    {
        FILE *file = fopen(filePath, "rb");
        if(file == NULL)
        {
            TI_ERROR("Could not open file from path: %s", filePath);
            *fileSize = 0;
        }
        fseek(file, 0, SEEK_END);
        *fileSize = (size_t)ftell(file);
        fseek(file, 0, SEEK_SET);

        fread(buffer, sizeof(u8), *fileSize, file);
        fclose(file);
    }

    u8* readFile(const char *filePath, size_t *fileSize)
    {
        FILE *file = fopen(filePath, "rb");
        if(file == NULL)
        {
            TI_ERROR("Could not open file from path: %s", filePath);
            *fileSize = 0;
            return NULL;
        }
        fseek(file, 0, SEEK_END);
        *fileSize = (size_t)ftell(file);
        fseek(file, 0, SEEK_SET);

        u8 *buffer = (u8*)malloc(*fileSize * sizeof(u8));
        fread(buffer, sizeof(u8), *fileSize, file);
        fclose(file);

        return buffer;
    }
}