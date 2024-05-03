#pragma once

#include "queue_families.h"

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"



namespace ti::graphics
{
    struct Swapchain
    {
        VkSwapchainKHR swapchain;
        VkDevice device;
        uint32_t imageCount;
        #define MAX_IMAGE_COUNT 10
        VkImage images[MAX_IMAGE_COUNT];
        VkImageView imageViews[MAX_IMAGE_COUNT];
        VkFormat imageFormat;
        VkExtent2D extent;
    };
    void _createSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, GLFWwindow *window, VkSurfaceKHR surface, const QueueFamilies *queueFamilies, Swapchain *swapchain);
    void _destroySwapchain(Swapchain *swapchain);


    void _createVkSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, GLFWwindow *window, VkSurfaceKHR surface, const QueueFamilies *queueFamilies, VkSwapchainKHR *swapchain);
    void _destroyVkSwapchain(VkDevice device, VkSwapchainKHR *swapchain);

    void _chooseSwapSurfaceFormat(const VkSurfaceFormatKHR *availableFormats, uint32_t count, VkSurfaceFormatKHR *out);
    void _chooseSwapPresentMode(const VkPresentModeKHR *availablePresentModes, uint32_t count, VkPresentModeKHR *out);
    void _chooseSwapExtent(const VkSurfaceCapabilitiesKHR *capabilities, GLFWwindow *window, VkExtent2D *out);
}