#include "queue_families.h"

#include "tinker/core/debug.h"



namespace ti::graphics
{
    bool _isQueueFamiliesComplete(QueueFamilies *queueFamilies)
    {
        return (queueFamilies->graphicsIdx != UINT32_MAX)
            && (queueFamilies->presentIdx  != UINT32_MAX);
    }

    void _getQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilies *queueFamilies)
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, NULL);
        VkQueueFamilyProperties props[count];
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, props);

        for(int i = 0; i < count; i++)
        {
            const VkQueueFamilyProperties *p = &props[i];

            if(p->queueFlags & VK_QUEUE_GRAPHICS_BIT)
                queueFamilies->graphicsIdx = i;
            
            VkBool32 presentSupport;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if(presentSupport)
                queueFamilies->presentIdx = i;
            
            if(_isQueueFamiliesComplete(queueFamilies))
                break;
        }
        TI_ASSERT(_isQueueFamiliesComplete(queueFamilies), "");
    }
}