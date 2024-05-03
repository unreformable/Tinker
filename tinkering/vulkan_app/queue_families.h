#pragma once

#include "vulkan/vulkan.h"



namespace ti::graphics
{
    struct QueueFamilies
    {
        uint32_t graphicsIdx = UINT32_MAX;
        uint32_t presentIdx = UINT32_MAX;
    };

    bool _isQueueFamiliesComplete(QueueFamilies *queueFamilies);
    void _getQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilies *queueFamilies);
}