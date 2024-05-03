#pragma once

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

#include <optional> // std::optional
#include <vector>



/*
    HELPER FUNCTIONS
*/
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const
    {
        return graphicsFamily.has_value()
            && presentFamily.has_value();
    }
};
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    bool isAdequate() const
    {
        return !formats.empty() && !presentModes.empty();
    }
};
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

std::vector<VkImage> createSwapchainImages(VkDevice device, VkSwapchainKHR swapchain);
std::vector<VkImageView> createSwapchainImageViews(VkDevice device, VkFormat swapChainImageFormat, const std::vector<VkImage>& swapChainImages);
void destroySwapchainImageViews(VkDevice device, std::vector<VkImageView>& imageViews);



/*
    MAIN FUNCTIONS
*/
VkInstance createInstance();
void destroyInstance(VkInstance instance);

VkDebugUtilsMessengerEXT createDebugMessenger(VkInstance instance);
void destroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window);
void destroySurface(VkInstance instance, VkSurfaceKHR surface);

/**
 * @note Implicitly destroyed with instance
*/
VkPhysicalDevice createPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

VkDevice createDevice(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices);
void destroyDevice(VkDevice device);

VkSwapchainKHR createSwapchain(
    VkDevice device,
    VkSurfaceKHR surface,
    const QueueFamilyIndices& indices,
    const SwapChainSupportDetails& swapChainSupport,
    VkSurfaceFormatKHR surfaceFormat,
    VkPresentModeKHR presentMode,
    VkExtent2D extent);
void destroySwapchain(VkDevice device, VkSwapchainKHR swapchain);