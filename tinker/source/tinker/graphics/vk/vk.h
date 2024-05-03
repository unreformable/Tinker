#pragma once

#include "vulkan/vulkan.h"



namespace ti::graphics::vk
{
    void initalize()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = NULL;
        appInfo.pApplicationName = "Application name";
        appInfo.applicationVersion;
        appInfo.pEngineName;
        appInfo.engineVersion;
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo instanceInfo{};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pNext = NULL;
        // instanceInfo.flags
        instanceInfo.pApplicationInfo = &appInfo;
        // instanceInfo.enabledLayerCount;
        // instanceInfo.ppEnabledLayerNames;
        // instanceInfo.enabledExtensionCount;
        // instanceInfo.ppEnabledExtensionNames;
        VkInstance instance;
        vkCreateInstance(&instanceInfo, NULL, &instance);
    }

    void terminate()
    {

    }
}