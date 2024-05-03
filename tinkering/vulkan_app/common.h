#pragma once

#include "tinker/core/debug.h"

#include "vulkan/vulkan.h"



#define VK_CHECK_RESULT(x)  \
    if(x != VK_SUCCESS)     \
    {                       \
        TI_ASSERT(0, "");   \
    }

#define GET_ASSET_PATH(asset_name) "../../../tinkering/vulkan_app/" asset_name

namespace ti::graphics
{
    #ifdef NDEBUG
        static const bool ENABLE_VALIDATION_LAYERS = false;
    #else
        static const bool ENABLE_VALIDATION_LAYERS = true;
    #endif

    static const char *VALIDATION_LAYER_NAMES[]{
        "VK_LAYER_KHRONOS_validation"
    };
    static const uint32_t VALIDATION_LAYER_NAME_COUNT = sizeof(VALIDATION_LAYER_NAMES)/sizeof(VALIDATION_LAYER_NAMES[0]);

    static const char* DEVICE_EXTENSION_NAMES[]{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    static const uint32_t DEVICE_EXTENSION_NAME_COUNT = sizeof(DEVICE_EXTENSION_NAMES)/sizeof(DEVICE_EXTENSION_NAMES[0]);
}