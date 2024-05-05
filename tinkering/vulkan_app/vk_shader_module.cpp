#include "vk_shader_module.h"

#include "common.h"

#include <stdio.h>



namespace ti::graphics
{
    void _createVkShaderModule(VkDevice device, const char *filePath, VkShaderModule *module)
    {
        FILE* file = fopen(filePath, "rb");

        fseek(file, 0, SEEK_END);
        const size_t size = (size_t)ftell(file);
        fseek(file, 0, SEEK_SET);

        char buffer[size];
        fread(buffer, sizeof(buffer[0]), size, file);
        fclose(file);

        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.codeSize = size;
        createInfo.pCode = (uint32_t *)buffer;
        VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, NULL, module));
    }

    void _destroyVkShaderModule(VkDevice device, VkShaderModule *module)
    {
        vkDestroyShaderModule(device, *module, NULL);
    }
}