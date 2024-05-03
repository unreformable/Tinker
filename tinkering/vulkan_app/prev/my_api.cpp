#include "my_api.h"

#include "tinker/core/debug.h"

#include <algorithm> // Necessary for std::clamp
#include <cstring>
#include <iostream>
#include <limits> // Necessary for std::numeric_limits
#include <map> // std::multimap
#include <set>



// TODO: Better physical device choosing

#ifdef NDEBUG
    static const bool enableValidationLayers = false;
#else
    static const bool enableValidationLayers = true;
#endif

static const std::vector<const char*> validationLayerNames
{
    "VK_LAYER_KHRONOS_validation"
};

static const std::vector<const char*> deviceExtensionNames
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static std::vector<VkLayerProperties> getLayers()
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    return layers;
}

// static std::vector<VkExtensionProperties> getExtensions()
// {
//     uint32_t extensionCount = 0;
//     vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

//     std::vector<VkExtensionProperties> extensions(extensionCount);
//     vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

//     return extensions;
// }

// static void printExtensions()
// {
//     std::vector<VkExtensionProperties> extensions = getExtensions();

//     std::cout << "Available extensions:\n";
//     for(const VkExtensionProperties& e : extensions)
//     {
//         std::cout << "  " << e.extensionName << '\n';
//     }
// }

/**
 * @brief Prototype of extension function
*/
VkResult vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func == NULL)
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

/**
 * @brief Prototype of extension function
*/
void vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func != NULL)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    // if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    // {
    //     return VK_FALSE;
    // }
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

static bool checkValidationLayersSupport()
{
    const std::vector<VkLayerProperties> layers = getLayers();

    for(const char* layerName : validationLayerNames)
    {
        bool layerFound = false;

        for(const VkLayerProperties& layerProperties : layers)
        {
            if(strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            TI_ERROR("Layer %s must be available", layerName);
            return false;
        }
    }

    return true;
}

static std::vector<const char*> getRequiredExtensionNames()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if(enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    // createInfo.pUserData = NULL; // Optional
}

static bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensionNames.begin(), deviceExtensionNames.end());

    for(const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

static bool isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);
    bool swapChainAdequate = false;
    if(extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = swapChainSupport.isAdequate();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

static int rateDeviceSuitability(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    if(!isPhysicalDeviceSuitable(physicalDevice, surface))
    {
        return 0;
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    // Application can't function without geometry shaders
    // if(!deviceFeatures.geometryShader)
    // {
    //     return 0;
    // }

    int score = 0;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}














































QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        // TODO: Prefer support of both queue families at once for better performance
        if(presentSupport)
        {
            indices.presentFamily = i;
        }
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        if(indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if(formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
        && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    // TODO: Rank formats based on how "good" they are
    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    // TODO: Always use VK_PRESENT_MODE_FIFO_KHR when energy usage is important (e.g. on mobile devices)

    for(const auto& availablePresentMode : availablePresentModes) 
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
    // TODO: Is swap extent the resolution choosen in games?

    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            (uint32_t)width,
            (uint32_t)height
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

std::vector<VkImage> createSwapchainImages(VkDevice device, VkSwapchainKHR swapchain)
{
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    std::vector<VkImage> swapchainImages(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
    return swapchainImages;
}

std::vector<VkImageView> createSwapchainImageViews(VkDevice device, VkFormat swapChainImageFormat, const std::vector<VkImage>& swapChainImages)
{
    std::vector<VkImageView> swapChainImageViews(swapChainImages.size());
    for(size_t i = 0; i < swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1; // use more layers for stereographic 3D app

        if(vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
        {
            TI_ASSERT(false, "Successfully create image views");
        }
    }

    return swapChainImageViews;
}

void destroySwapchainImageViews(VkDevice device, std::vector<VkImageView>& imageViews)
{
    for(auto imageView : imageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }
}

















































VkInstance createInstance()
{
    if(enableValidationLayers && !checkValidationLayersSupport())
    {
        TI_ASSERT(false, "Validation layers requested must be available");
    }
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    // appInfo.pNext = NULL;
    appInfo.pApplicationName = "Hello vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Engine?";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    // createInfo.pNext = NULL;
    // createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    // createInfo.enabledLayerCount = 0;
    // createInfo.ppEnabledLayerNames = NULL;
    // createInfo.enabledExtensionCount = glfwExtensionCount;
    // createInfo.ppEnabledExtensionNames = glfwExtensions;

    if(enableValidationLayers)
    {
        createInfo.enabledLayerCount = (uint32_t)validationLayerNames.size();
        createInfo.ppEnabledLayerNames = validationLayerNames.data();
    }
    
    std::vector<const char*> extensions = getRequiredExtensionNames();
    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = (uint32_t)validationLayerNames.size();
        createInfo.ppEnabledLayerNames = validationLayerNames.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }

    VkInstance instance;
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        TI_ASSERT(false, "Instance is created");
    }

    return instance;
}

void destroyInstance(VkInstance instance)
{
    vkDestroyInstance(instance, NULL);
}

VkDebugUtilsMessengerEXT createDebugMessenger(VkInstance instance)
{
    VkDebugUtilsMessengerEXT debugMessenger;
    if(enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        if(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        {
            TI_ASSERT(false, "Debug messenger must be set up");
        }
    }

    return debugMessenger;
}

void destroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger)
{
    if(enableValidationLayers)
    {
        vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window)
{
    VkSurfaceKHR surface;
    if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        TI_ASSERT(false, "Window surface must be created");
    }
    return surface;
}

void destroySurface(VkInstance instance, VkSurfaceKHR surface)
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

VkPhysicalDevice createPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if(deviceCount == 0)
    {
        TI_ASSERT(false, "At least one GPU must support Vulkan");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for(const VkPhysicalDevice physicalDevice : physicalDevices)
    {
        int score = rateDeviceSuitability(physicalDevice, surface);

        if(score > 0)
        {
            candidates.insert(std::make_pair(score, physicalDevice));
        }
    }

    if(candidates.empty())
    {
        TI_ASSERT(false, "At least once physical device must be suitable");
    }

    physicalDevice = candidates.rbegin()->second;

    return physicalDevice;
}

VkDevice createDevice(VkPhysicalDevice physicalDevice, const QueueFamilyIndices& indices)
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = (uint32_t)deviceExtensionNames.size();
    createInfo.ppEnabledExtensionNames = deviceExtensionNames.data();

    if(enableValidationLayers)
    {
        createInfo.enabledLayerCount = (uint32_t)validationLayerNames.size();
        createInfo.ppEnabledLayerNames = validationLayerNames.data();
    }

    VkDevice device;
    if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        TI_ASSERT(false, "Logical device must be created");
    }

    return device;
}

void destroyDevice(VkDevice device)
{
    vkDestroyDevice(device, NULL);
}

VkSwapchainKHR createSwapchain(
    VkDevice device,
    VkSurfaceKHR surface,
    const QueueFamilyIndices& indices,
    const SwapChainSupportDetails& swapChainSupport,
    VkSurfaceFormatKHR surfaceFormat,
    VkPresentModeKHR presentMode,
    VkExtent2D extent)
{
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if(swapChainSupport.capabilities.maxImageCount > 0 // 0 is a special value that means that there is no maximum
    && swapChainSupport.capabilities.maxImageCount < imageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // 1 unless you make stereoscopic 3D application
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // render to image by using VK_IMAGE_USAGE_TRANSFER_DST_BIT

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if(indices.graphicsFamily != indices.presentFamily) // graphics and present families are the same
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // ownership of image is explicitly transferred between queues
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        // TODO: Make image ownership transferring between multiple queue families (low priority)

        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // image is transferred between queues "freely" (but costs performance)
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // alpha opaque means do not use blending between multiple windows
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE; // clipped ignores color of the pixels in window that is obscured by another window
    createInfo.oldSwapchain = VK_NULL_HANDLE; // old swap chain handle for recreation

    VkSwapchainKHR swapchain;
    if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        TI_ASSERT(false, "Swap chain must be created");
    }

    return swapchain;
}

void destroySwapchain(VkDevice device, VkSwapchainKHR swapchain)
{
    vkDestroySwapchainKHR(device, swapchain, nullptr);
}