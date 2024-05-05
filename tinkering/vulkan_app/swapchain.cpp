#include "swapchain.h"

#include "common.h"
#include "queue_families.h"

#include <algorithm>



namespace ti::graphics
{
    void _createSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, GLFWwindow *window, VkSurfaceKHR surface, const QueueFamilies *queueFamilies, Swapchain *swapchain)
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
        VkSurfaceFormatKHR formats[formatCount];
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
        VkPresentModeKHR presentModes[presentModeCount];
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes);

        VkSurfaceFormatKHR surfaceFormat;
        _chooseSwapSurfaceFormat(formats, formatCount, &surfaceFormat);
        VkPresentModeKHR presentMode;
        _chooseSwapPresentMode(presentModes, presentModeCount, &presentMode);
        VkExtent2D extent;
        _chooseSwapExtent(&capabilities, window, &extent);

        uint32_t minImageCount = capabilities.minImageCount + 1;
        if(capabilities.maxImageCount > 0
        && minImageCount > capabilities.maxImageCount)
        {
            minImageCount = capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.surface = surface;
        createInfo.minImageCount = minImageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        //createInfo.imageSharingMode;
        //createInfo.queueFamilyIndexCount;
        //createInfo.pQueueFamilyIndices;
        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        const uint32_t queueFamilyIndices[] = { queueFamilies->graphicsIdx, queueFamilies->presentIdx };
        if(queueFamilies->graphicsIdx != queueFamilies->presentIdx)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = NULL;
        }

        VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain->swapchain));

        swapchain->device = device;
        swapchain->imageFormat = surfaceFormat.format;
        swapchain->extent = extent;

        vkGetSwapchainImagesKHR(device, swapchain->swapchain, &swapchain->imageCount, NULL);
        //swapchain->images = (VkImage *)malloc(swapchain->imageCount * sizeof(VkImage));
        vkGetSwapchainImagesKHR(device, swapchain->swapchain, &swapchain->imageCount, swapchain->images);

        //swapchain->imageViews = (VkImageView *)malloc(swapchain->imageCount * sizeof(VkImageView));
        for(uint32_t i = 0; i < swapchain->imageCount; i++)
        {
            VkImageViewCreateInfo imageViewCreateInfo;
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = NULL;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.image = swapchain->images[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = surfaceFormat.format;
            imageViewCreateInfo.components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            };
            imageViewCreateInfo.subresourceRange = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };

            VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, NULL, &swapchain->imageViews[i]));
        }
    }

    void _destroySwapchain(Swapchain *swapchain)
    {
        for(int i = 0; i < swapchain->imageCount; i++)
        {
            vkDestroyImageView(swapchain->device, swapchain->imageViews[i], NULL);
        }
        //free(swapchain->imageViews);
        //free(swapchain->images);
        vkDestroySwapchainKHR(swapchain->device, swapchain->swapchain, NULL);
    }

    void _createVkSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, GLFWwindow *window, VkSurfaceKHR surface, const QueueFamilies *queueFamilies, VkSwapchainKHR *swapchain)
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
        VkSurfaceFormatKHR formats[formatCount];
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
        VkPresentModeKHR presentModes[presentModeCount];
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes);

        VkSurfaceFormatKHR surfaceFormat;
        _chooseSwapSurfaceFormat(formats, formatCount, &surfaceFormat);
        VkPresentModeKHR presentMode;
        _chooseSwapPresentMode(presentModes, presentModeCount, &presentMode);
        VkExtent2D extent;
        _chooseSwapExtent(&capabilities, window, &extent);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if(capabilities.maxImageCount > 0
        && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        //createInfo.imageSharingMode;
        //createInfo.queueFamilyIndexCount;
        //createInfo.pQueueFamilyIndices;
        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        const uint32_t queueFamilyIndices[] = { queueFamilies->graphicsIdx, queueFamilies->presentIdx };
        if(queueFamilies->graphicsIdx != queueFamilies->presentIdx)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = NULL;
        }

        VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &createInfo, NULL, swapchain));
    }

    void _destroyVkSwapchain(VkDevice device, VkSwapchainKHR *swapchain)
    {
        vkDestroySwapchainKHR(device, *swapchain, NULL);
    }

    void _chooseSwapSurfaceFormat(const VkSurfaceFormatKHR *availableFormats, uint32_t count, VkSurfaceFormatKHR *out)
    {
        for(int i = 0; i < count; i++)
        {
            const auto *availableFormat = &availableFormats[i];

            if(availableFormat->format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // srgb colors are better perceived by us
            {
                *out = *availableFormat;
                return;
            }
        }

        *out = availableFormats[0];
    }

    void _chooseSwapPresentMode(const VkPresentModeKHR *availablePresentModes, uint32_t count, VkPresentModeKHR *out)
    {
        for(int i = 0; i < count; i++)
        {
            VkPresentModeKHR availablePresentMode = availablePresentModes[i];

            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) // no tearing with low latence but costs more energy (if energy is a concern use VK_PRESENT_MODE_FIFO_KHR)
            {
                *out = availablePresentMode;
                return;
            }
        }

        *out = VK_PRESENT_MODE_FIFO_KHR;
    }

    void _chooseSwapExtent(const VkSurfaceCapabilitiesKHR *capabilities, GLFWwindow *window, VkExtent2D *out)
    {
        if(capabilities->currentExtent.width != UINT32_MAX)
        {
            *out = capabilities->currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent{ (uint32_t)width, (uint32_t)height };
            actualExtent.width = std::clamp(actualExtent.width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);

            *out = actualExtent;
        }
    }
}