#pragma once

#include "vulkan/vulkan.h"



namespace ti::graphics
{
    void _createVkShaderModule(VkDevice device, const char *filePath, VkShaderModule *module);
    void _destroyVkShaderModule(VkDevice device, VkShaderModule *module);
}