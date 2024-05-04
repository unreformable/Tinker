#include "graphics.h"

#include "common.h"
#include "queue_families.h"
#include "swapchain.h"
#include "vk_shader_module.h"

#include "tinker/core/debug.h"

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"



// void initalize()
// {
//     _initalizeGlfw();
//     _initalizeInstance();
//     _initalizeDebugUtilsMessenger();
//     _getPhysicalDeviceAndQueueFamilies();
//     _initalizeDeviceAndQueues();
// }

// void terminate()
// {
//     _terminateDeviceAndQueues();
//     _terminateDebugUtilsMessenger();
//     _terminateInstance();
//     _terminateGlfw();
// }

namespace ti::graphics
{
    void _initalizeGlfw()
    {
        glfwSetErrorCallback([](int code, const char *desc)
        {
            TI_ERROR("GLFW error: %d, %s", code, desc);
        });
        if(glfwInit() == GLFW_FALSE)
        {
            TI_ASSERT(0, "");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    }

    void _terminateGlfw()
    {
        glfwTerminate();
    }

    void _createWindow(int width, int height, const char *title, GLFWwindow **window, GLFWframebuffersizefun framebufferSizeCallback)
    {
        *window = glfwCreateWindow(width, height, title, NULL, NULL);
        glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);
    }

    void _destroyWindow(GLFWwindow *window)
    {
        glfwDestroyWindow(window);
    }

    void _createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface)
    {
        VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, NULL, surface));
    }

    void _destroySurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        vkDestroySurfaceKHR(instance, *surface, NULL);
    }

    VkResult _vkCreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
        const VkAllocationCallbacks *allocator,
        VkDebugUtilsMessengerEXT *debugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if(func == NULL)
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        return func(instance, createInfo, allocator, debugMessenger);
    }

    void _vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *allocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if(func != NULL)
        {
            func(instance, debugMessenger, allocator);
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
        void *userData)
    {
        if(messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            return VK_FALSE;
        }
        TI_ERROR("Vulkan validator layers: %s\n", callbackData->pMessage);

        return VK_FALSE;
    }

    void _populateDebugUtilsMessangerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT *debugCreateInfo)
    {
        debugCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo->pNext = NULL;
        debugCreateInfo->flags = 0;
        debugCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo->pfnUserCallback = _debugCallback;
        debugCreateInfo->pUserData = NULL;
    }

    void _createDebugUtilsMessenger(VkInstance instance, VkDebugUtilsMessengerEXT *messenger)
    {
        if(!ENABLE_VALIDATION_LAYERS)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        _populateDebugUtilsMessangerCreateInfoEXT(&createInfo);
        VK_CHECK_RESULT(_vkCreateDebugUtilsMessengerEXT(instance, &createInfo, NULL, messenger));
    }

    void _destroyDebugUtilsMessenger(VkInstance instance, VkDebugUtilsMessengerEXT *messenger)
    {
        if(!ENABLE_VALIDATION_LAYERS)
            return;
        
        _vkDestroyDebugUtilsMessengerEXT(instance, *messenger, NULL);
    }

    void _createInstance(VkInstance *instance)
    {
        // Checks validation layers support
        if(ENABLE_VALIDATION_LAYERS)
        {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, NULL);
            VkLayerProperties layers[layerCount];
            vkEnumerateInstanceLayerProperties(&layerCount, layers);

            for(const char *validationLayerName : VALIDATION_LAYER_NAMES)
            {
                bool layerFound = false;
                for(int i = 0; i < layerCount; i++)
                {
                    const VkLayerProperties *layer = &layers[i];

                    if(strcmp(validationLayerName, layer->layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if(!layerFound)
                {
                    TI_ASSERT(0, "");
                }
            }
        }

        // Check extensions support
        uint32_t glfwExtensionNameCount;
        const char **glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionNameCount);
        {
            uint32_t extensionCount;
            vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
            VkExtensionProperties extensions[extensionCount];
            vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);

            for(uint32_t i = 0; i < glfwExtensionNameCount; i++)
            {
                const char *glfwExtensionName = glfwExtensionNames[i];
                bool extensionFound = false;
                for(uint32_t j = 0; j < extensionCount; j++)
                {
                    if(strcmp(glfwExtensionName, extensions[j].extensionName) == 0)
                    {
                        extensionFound = true;
                        break;
                    }
                }

                if(!extensionFound)
                {
                    TI_ASSERT(0, "");
                }
            }
        }

        // Prepare extensions
        #ifdef NDEBUG
            const uint32_t extensionNameCount = glfwExtensionNameCount;
            const char *extensionNames[extensionNameCount];
        #else
            const uint32_t extensionNameCount = glfwExtensionNameCount + 1;
            const char *extensionNames[extensionNameCount];
            extensionNames[extensionNameCount - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        #endif
        for(uint32_t i = 0; i < glfwExtensionNameCount; i++)
        {
            extensionNames[i] = glfwExtensionNames[i];
        }

        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = NULL;
        appInfo.pApplicationName = "Application name";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.pEngineName = "Engine name";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        //createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfo;
        //createInfo.enabledLayerCount = g::validationLayerNameCount;
        //createInfo.ppEnabledLayerNames = g::validationLayerNames;
        createInfo.enabledExtensionCount = extensionNameCount;
        createInfo.ppEnabledExtensionNames = extensionNames;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if(ENABLE_VALIDATION_LAYERS)
        {
            _populateDebugUtilsMessangerCreateInfoEXT(&debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
            createInfo.enabledLayerCount = VALIDATION_LAYER_NAME_COUNT;
            createInfo.ppEnabledLayerNames = VALIDATION_LAYER_NAMES;
        }
        else
        {
            createInfo.pNext = NULL;
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = NULL;
        }

        VK_CHECK_RESULT(vkCreateInstance(&createInfo, NULL, instance));
    }

    void _destroyInstance(VkInstance *instance)
    {
        vkDestroyInstance(*instance, NULL);
    }

    void _getPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice *outPhysicalDevice)
    {
        uint32_t count;
        vkEnumeratePhysicalDevices(instance, &count, NULL);
        if(count == 0) TI_ASSERT(0, "");
        VkPhysicalDevice physicalDevices[count];
        vkEnumeratePhysicalDevices(instance, &count, physicalDevices);

        int scores[count];
        for(int i = 0; i < count; i++)
        {
            const VkPhysicalDevice physicalDevice = physicalDevices[i];

            QueueFamilies queueFamilies;
            _getQueueFamilies(physicalDevice, surface, &queueFamilies);
            if(!_isQueueFamiliesComplete(&queueFamilies))
            {
                scores[i] = 0;
                continue;
            }

            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL);
            VkExtensionProperties availableExtensions[extensionCount];
            vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, availableExtensions);
            for(const char *deviceExtensionName : DEVICE_EXTENSION_NAMES)
            {
                bool extensionSupported = false;
                for(int i = 0; i < extensionCount; i++)
                {
                    const VkExtensionProperties *extension = &availableExtensions[i];

                    if(strcmp(deviceExtensionName, extension->extensionName) == 0)
                    {
                        extensionSupported = true;
                        break;
                    }
                }
                TI_ASSERT(extensionSupported == true, "");
            }

            uint32_t swapchainFormatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &swapchainFormatCount, NULL);
            uint32_t swapchainPresentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &swapchainPresentModeCount, NULL);
            const bool swapChainAdequate = (swapchainFormatCount > 0 && swapchainPresentModeCount > 0);
            TI_ASSERT(swapChainAdequate == true, "");

            VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceFeatures(physicalDevice, &features);
            if(!features.geometryShader) // Application can't function without geometry shaders
            {
                scores[i] = 0;
                continue;
            }

            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physicalDevice, &props);
            int score = 0;
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) // discrete GPUs have a significant performance advantage
                score += 1000;

            score += props.limits.maxImageDimension2D; // maximum possible size of textures affects graphics quality
            
            scores[i] = score;
        }
        
        int best = 0;
        for(int i = 1; i < count; i++)
        {
            if(scores[i] > scores[best])
                best = i;
        }
        TI_ASSERT(scores[best] != 0, "");

        *outPhysicalDevice = physicalDevices[best];
    }

    void _createDevice(const QueueFamilies* queueFamilies, VkPhysicalDevice physicalDevice, VkDevice *device)
    {
        float queuePriority = 1.0f;
        uint32_t queueCreateInfoCount;
        VkDeviceQueueCreateInfo queueCreateInfos[2];
        if(queueFamilies->graphicsIdx == queueFamilies->presentIdx)
        {
            queueCreateInfoCount = 1;

            queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[0].pNext = NULL;
            queueCreateInfos[0].flags = 0;
            queueCreateInfos[0].queueFamilyIndex = queueFamilies->graphicsIdx;
            queueCreateInfos[0].queueCount = 1;
            queueCreateInfos[0].pQueuePriorities = &queuePriority;
        }
        else
        {
            queueCreateInfoCount = 2;

            queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[0].pNext = NULL;
            queueCreateInfos[0].flags = 0;
            queueCreateInfos[0].queueFamilyIndex = queueFamilies->graphicsIdx;
            queueCreateInfos[0].queueCount = 1;
            queueCreateInfos[0].pQueuePriorities = &queuePriority;

            queueCreateInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[1].pNext = NULL;
            queueCreateInfos[1].flags = 0;
            queueCreateInfos[1].queueFamilyIndex = queueFamilies->presentIdx;
            queueCreateInfos[1].queueCount = 1;
            queueCreateInfos[1].pQueuePriorities = &queuePriority;
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.queueCreateInfoCount = queueCreateInfoCount;
        createInfo.pQueueCreateInfos = queueCreateInfos;
        //createInfo.enabledLayerCount = 0;
        //createInfo.ppEnabledLayerNames = NULL;
        createInfo.enabledExtensionCount = DEVICE_EXTENSION_NAME_COUNT;
        createInfo.ppEnabledExtensionNames = DEVICE_EXTENSION_NAMES;
        createInfo.pEnabledFeatures = &deviceFeatures;

        if(ENABLE_VALIDATION_LAYERS)
        {
            createInfo.enabledLayerCount = VALIDATION_LAYER_NAME_COUNT;
            createInfo.ppEnabledLayerNames = VALIDATION_LAYER_NAMES;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = NULL;
        }

        VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &createInfo, NULL, device));
    }

    void _getQueues(VkDevice device, const QueueFamilies *queueFamilies, VkQueue *graphicsQueue, VkQueue *presentQueue)
    {
        vkGetDeviceQueue(device, queueFamilies->graphicsIdx, 0, graphicsQueue);
        vkGetDeviceQueue(device, queueFamilies->presentIdx,  0, presentQueue);
    }

    void _destroyDevice(VkDevice *device)
    {
        vkDestroyDevice(*device, NULL);
    }

    void _createRenderPass(VkDevice device, const Swapchain *swapchain, VkRenderPass *renderPass)
    {
        VkAttachmentDescription colorAttachment;
        colorAttachment.flags = 0;
        colorAttachment.format = swapchain->imageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear color and depth data before rendering
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Store color and depth data after rendering
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // We don't care about layout type before rendering
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Image must be ready for presentation after rendering

        VkAttachmentReference colorAttachmentRef;
        colorAttachmentRef.attachment = 0; // Attachment idx
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Attachment used as color buffer

        VkSubpassDescription subpass;
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = NULL; // Attachments read from shader
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef; // Idx of this array is referenced from the fragment shader with the output color directive
        subpass.pResolveAttachments = NULL; // Attachments used for multisampling color attachments
        subpass.pDepthStencilAttachment = NULL; // Attachments used for depth and stencil data
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments = NULL; // Attachments not used by this subpass, but other subpass may use

        VkSubpassDependency subpassDependency;
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0; // Always higher that src subpass to prevent cycles in dependency graph
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency.dependencyFlags = 0;

        VkRenderPassCreateInfo renderPassCreateInfo;
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.pNext = NULL;
        renderPassCreateInfo.flags = 0;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpassDependency;

        VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassCreateInfo, NULL, renderPass))
    }

    void _destroyRenderPass(VkDevice device, VkRenderPass *renderPass)
    {
        vkDestroyRenderPass(device, *renderPass, NULL);
    }

    void _createGraphicsPipeline(VkDevice device, const Swapchain *swapchain, VkRenderPass *renderPass, VkPipeline *graphicsPipeline)
    {
        VkShaderModule vsModule;
        _createVkShaderModule(device, GET_ASSET_PATH("shaders/vert.spv"), &vsModule);
        VkShaderModule fsModule;
        _createVkShaderModule(device, GET_ASSET_PATH("shaders/frag.spv"), &fsModule);

        VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo;
        vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageCreateInfo.pNext = NULL;
        vertShaderStageCreateInfo.flags = 0;
        vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageCreateInfo.module = vsModule;
        vertShaderStageCreateInfo.pName = "main"; // specify which function is an entry point (among many possible inside single shader)
        vertShaderStageCreateInfo.pSpecializationInfo = NULL; // used to set constants for shader at compile time (e.g. can eliminate if statements)

        VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo;
        fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageCreateInfo.pNext = NULL;
        fragShaderStageCreateInfo.flags = 0;
        fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageCreateInfo.module = fsModule;
        fragShaderStageCreateInfo.pName = "main"; // specify which function is an entry point (among many possible inside single shader)
        fragShaderStageCreateInfo.pSpecializationInfo = NULL; // used to set constants for shader at compile time (e.g. can eliminate if statements)

        const VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragShaderStageCreateInfo };



        const VkDynamicState dynamicStates[]{
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        const uint32_t dynamicStateCount = sizeof(dynamicStates)/sizeof(dynamicStates[0]);

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.pNext = NULL;
        dynamicStateCreateInfo.flags = 0;
        dynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
        dynamicStateCreateInfo.pDynamicStates = dynamicStates;

        VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
        vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputCreateInfo.pNext = NULL;
        vertexInputCreateInfo.flags = 0;
        vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
        vertexInputCreateInfo.pVertexBindingDescriptions = NULL;
        vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
        vertexInputCreateInfo.pVertexAttributeDescriptions = NULL;

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
        inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyCreateInfo.pNext = NULL;
        inputAssemblyCreateInfo.flags = 0;
        inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE; // May be used to break up strip topology into lines and triangles

        // VkViewport viewport;
        // viewport.x = 0.0f;
        // viewport.y = 0.0f;
        // viewport.width = swapchain->extent.width; // Use swapchain images size, because may differ from window size
        // viewport.height = swapchain->extent.height; // Use swapchain images size, because may differ from window size
        // viewport.minDepth = 0.0f; // May be exchanged with the max depth in the future to increase precision of z value (like Godot Engine did; more one Godot Engine page)
        // viewport.maxDepth = 1.0f; // May be exchanged with the min depth in the future to increase precision of z value (like Godot Engine did; more one Godot Engine page)

        // VkRect2D scissor;
        // scissor.offset = { 0, 0 };
        // scissor.extent = swapchain->extent;

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.pNext = NULL;
        viewportStateCreateInfo.flags = 0;
        viewportStateCreateInfo.viewportCount = 1;
        viewportStateCreateInfo.pViewports = NULL; //&viewport; // Viewport specified = immutable viewport, otherwise dynamic
        viewportStateCreateInfo.scissorCount = 1;
        viewportStateCreateInfo.pScissors = NULL; //&scissor; // Scissor specified = immutable scissor, otherwise dynamic

        VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo;
        rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerCreateInfo.pNext = NULL;
        rasterizerCreateInfo.flags = 0;
        rasterizerCreateInfo.depthClampEnable = VK_FALSE; // Clamp fragments beyond the near and far planes to planes?
        rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE; // Discard fragments?
        rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizerCreateInfo.depthBiasEnable = VK_FALSE; // May be used for shadow mapping
        rasterizerCreateInfo.depthBiasConstantFactor = 0.0f; // May be used for shadow mapping
        rasterizerCreateInfo.depthBiasClamp = 0.0f; // May be used for shadow mapping
        rasterizerCreateInfo.depthBiasSlopeFactor = 0.0f; // May be used for shadow mapping
        rasterizerCreateInfo.lineWidth = 1.0f;

        VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo;
        multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingCreateInfo.pNext = NULL;
        multisamplingCreateInfo.flags = 0;
        multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
        multisamplingCreateInfo.minSampleShading = 1.0f;
        multisamplingCreateInfo.pSampleMask = NULL;
        multisamplingCreateInfo.alphaToCoverageEnable = VK_FALSE;
        multisamplingCreateInfo.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo;
        colorBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendingCreateInfo.pNext = NULL;
        colorBlendingCreateInfo.flags = 0;
        colorBlendingCreateInfo.logicOpEnable = VK_FALSE; // WARNING: If enabled - disables blendEnable in VkPipelineColorBlendAttachmentState
        colorBlendingCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendingCreateInfo.attachmentCount = 1;
        colorBlendingCreateInfo.pAttachments = &colorBlendAttachment;
        colorBlendingCreateInfo.blendConstants[0] = 0.0f;
        colorBlendingCreateInfo.blendConstants[1] = 0.0f;
        colorBlendingCreateInfo.blendConstants[2] = 0.0f;
        colorBlendingCreateInfo.blendConstants[3] = 0.0f;



        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pNext = NULL;
        pipelineLayoutCreateInfo.flags = 0;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = NULL;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = NULL;

        VkPipelineLayout pipelineLayout;
        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout));



        VkGraphicsPipelineCreateInfo pipelineCreateInfo;
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.pNext = NULL;
        pipelineCreateInfo.flags = 0;
        pipelineCreateInfo.stageCount = 2;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
        pipelineCreateInfo.pTessellationState = NULL;
        pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
        pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
        pipelineCreateInfo.pDepthStencilState = NULL;
        pipelineCreateInfo.pColorBlendState = &colorBlendingCreateInfo;
        pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
        pipelineCreateInfo.layout = pipelineLayout;
        pipelineCreateInfo.renderPass = *renderPass;
        pipelineCreateInfo.subpass = 0; // Idx of subpass where graphics pipeline will be used
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Derive from already created pipeline
        pipelineCreateInfo.basePipelineIndex = -1; // Derive from pipeline that is about to be created

        VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, graphicsPipeline));

        vkDestroyPipelineLayout(device, pipelineLayout, NULL);
        _destroyVkShaderModule(device, &fsModule);
        _destroyVkShaderModule(device, &vsModule);
    }

    void _destroyGraphicsPipeline(VkDevice device, VkPipeline *graphicsPipeline)
    {
        vkDestroyPipeline(device, *graphicsPipeline, NULL);
    }

    void _createFramebuffers(VkDevice device, VkRenderPass renderPass, const Swapchain *swapchain, VkFramebuffer *framebuffers)
    {
        for(uint32_t i = 0; i < swapchain->imageCount; i++)
        {
            VkImageView attachments[] = { swapchain->imageViews[i] };

            VkFramebufferCreateInfo framebufferCreateInfo;
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.pNext = NULL;
            framebufferCreateInfo.flags = 0;
            framebufferCreateInfo.renderPass = renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = swapchain->extent.width;
            framebufferCreateInfo.height = swapchain->extent.height;
            framebufferCreateInfo.layers = 1;

            VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCreateInfo, NULL, &framebuffers[i]))
        }
    }

    void _destroyFramebuffers(VkDevice device, VkFramebuffer *framebuffers, uint32_t framebufferCount)
    {
        for(uint32_t i = 0; i < framebufferCount; i++)
        {
            vkDestroyFramebuffer(device, framebuffers[i], NULL);
        }
    }

    void _createCommandPool(VkDevice device, const QueueFamilies *queueFamilies, VkCommandPool *commandPool)
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo;
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = NULL;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Command buffers can be reset individually
        commandPoolCreateInfo.queueFamilyIndex = queueFamilies->graphicsIdx;

        VK_CHECK_RESULT(vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, commandPool))
    }

    void _destroyCommandPool(VkDevice device, VkCommandPool *commandPool)
    {
        vkDestroyCommandPool(device, *commandPool, NULL);
    }

    void _allocateCommandBuffers(VkDevice device, VkCommandPool commandPool, VkCommandBuffer *commandBuffers)
    {
        VkCommandBufferAllocateInfo allocInfo;
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Can be submitted for execution, but not called from other command buffers (secondary is opposite)
        allocInfo.commandBufferCount = 2;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers))
    }

    void _recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIdx, VkFramebuffer *framebuffers, const Swapchain *swapchain, VkRenderPass renderPass, VkPipeline graphicsPipeline)
    {
        VkCommandBufferBeginInfo beginInfo;
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = NULL;
        beginInfo.flags = 0; // These flags can be usefull (read whenever coming back here)
        beginInfo.pInheritanceInfo = NULL; // Used only from secondary buffers to specify which state to inherit

        VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo))

        VkClearValue clearValue;
        clearValue.color = { 0.0f, 0.0f, 0.0f, 0.0f };
        clearValue.depthStencil = { 0.0f, 0 };

        VkRenderPassBeginInfo renderPassInfo;
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.pNext = NULL;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = framebuffers[imageIdx];
        renderPassInfo.renderArea.extent = swapchain->extent;
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); // Execute render pass commands from primary buffer (the other option is to execute commands from secondary buffer)

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = swapchain->extent.width; // Use swapchain images size, because may differ from window size
        viewport.height = swapchain->extent.height; // Use swapchain images size, because may differ from window size
        viewport.minDepth = 0.0f; // May be exchanged with the max depth in the future to increase precision of z value (like Godot Engine did; more one Godot Engine page)
        viewport.maxDepth = 1.0f; // May be exchanged with the min depth in the future to increase precision of z value (like Godot Engine did; more one Godot Engine page)
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset = { 0, 0 };
        scissor.extent = swapchain->extent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);
        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer))
    }

    void _createSyncObjects(VkDevice device, VkSemaphore *imageAvailableSemaphores, VkSemaphore *renderFinishedSemaphores, VkFence *inFlightFences)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = NULL;
        semaphoreCreateInfo.flags = 0;

        VkFenceCreateInfo fenceCreateInfo;
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = NULL;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Fence is signaled at the start (used to e.g. fence wait at the beginning)

        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &imageAvailableSemaphores[i]))
            VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &renderFinishedSemaphores[i]))
            VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, NULL, &inFlightFences[i]))
        }
    }

    void _destroySyncObjects(VkDevice device, VkSemaphore *imageAvailableSemaphores, VkSemaphore *renderFinishedSemaphores, VkFence *inFlightFences)
    {
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(device, imageAvailableSemaphores[i], NULL);
            vkDestroySemaphore(device, renderFinishedSemaphores[i], NULL);
            vkDestroyFence(device, inFlightFences[i], NULL);
        }
    }

    void _recreateSwapchainAndFramebuffers(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        GLFWwindow *window,
        VkSurfaceKHR surface,
        QueueFamilies *queueFamilies,
        Swapchain *swapchain,
        VkRenderPass renderPass,
        VkFramebuffer *framebuffers)
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while(width == 0 || height == 0)
        {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
            if(glfwWindowShouldClose(window))
		        return;
        }

        vkDeviceWaitIdle(device);

        _destroyFramebuffers(device, framebuffers, swapchain->imageCount);
        _destroySwapchain(swapchain);

        _createSwapchain(physicalDevice, device, window, surface, queueFamilies, swapchain);
        _createFramebuffers(device, renderPass, swapchain, framebuffers);
    }

    // GLOBAL VARIABLES //

    struct Global
    {
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;

        GLFWwindow *window;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice;
        QueueFamilies queueFamilies;

        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        Swapchain swapchain;

        VkRenderPass renderPass;
        VkPipeline graphicsPipeline;

        #define MAX_FRAMEBUFFER_COUNT 10
        VkFramebuffer framebuffers[MAX_FRAMEBUFFER_COUNT];

        VkCommandPool commandPool;
        VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];

        VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
        VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
        VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];

        bool framebufferResized = false;

        uint32_t currentFrame = 0;
    } g;

    // HEADER FILE FUNCTIONS DEFINITIONS //

    void initalize()
    {
        _initalizeGlfw();
        _createWindow(1280, 720, "Hello, Vulkan!", &g.window, [](GLFWwindow* w, int width, int height)
        {
            g.framebufferResized = true;
        });

        _createInstance(&g.instance);
        _createDebugUtilsMessenger(g.instance, &g.debugMessenger);

        _createSurface(g.instance, g.window, &g.surface);

        _getPhysicalDevice(g.instance, g.surface, &g.physicalDevice);
        _getQueueFamilies(g.physicalDevice, g.surface, &g.queueFamilies);

        _createDevice(&g.queueFamilies, g.physicalDevice, &g.device);
        _getQueues(g.device, &g.queueFamilies, &g.graphicsQueue, &g.presentQueue);

        _createSwapchain(g.physicalDevice, g.device, g.window, g.surface, &g.queueFamilies, &g.swapchain);

        _createRenderPass(g.device, &g.swapchain, &g.renderPass);
        _createGraphicsPipeline(g.device, &g.swapchain, &g.renderPass, &g.graphicsPipeline);

        _createFramebuffers(g.device, g.renderPass, &g.swapchain, g.framebuffers);

        _createCommandPool(g.device, &g.queueFamilies, &g.commandPool);
        _allocateCommandBuffers(g.device, g.commandPool, g.commandBuffers);

        _createSyncObjects(g.device, g.imageAvailableSemaphores, g.renderFinishedSemaphores, g.inFlightFences);
    }

    void terminate()
    {
        vkDeviceWaitIdle(g.device); // Wait till all GPU commands are done (and then clean resources...)

        _destroySyncObjects(g.device, g.imageAvailableSemaphores, g.renderFinishedSemaphores, g.inFlightFences);

        _destroyCommandPool(g.device, &g.commandPool);

        _destroyFramebuffers(g.device, g.framebuffers, g.swapchain.imageCount);

        _destroyGraphicsPipeline(g.device, &g.graphicsPipeline);
        _destroyRenderPass(g.device, &g.renderPass);

        _destroySwapchain(&g.swapchain);

        _destroyDevice(&g.device);

        _destroySurface(g.instance, &g.surface);

        _destroyDebugUtilsMessenger(g.instance, &g.debugMessenger);
        _destroyInstance(&g.instance);

        _destroyWindow(g.window);
        _terminateGlfw();
    }

    bool shouldClose()
    {
        return glfwWindowShouldClose(g.window);
    }

    void pollEvents()
    {
        glfwPollEvents();
    }

    void drawFrame()
    {
        VkResult result;

        const VkCommandBuffer commandBuffer = g.commandBuffers[g.currentFrame];
        const VkSemaphore imageAvailableSemaphore = g.imageAvailableSemaphores[g.currentFrame];
        const VkSemaphore renderFinishedSemaphore = g.renderFinishedSemaphores[g.currentFrame];
        const VkFence inFlightFence = g.inFlightFences[g.currentFrame];

        vkWaitForFences(g.device, 1, &inFlightFence, VK_TRUE, UINT64_MAX); // Wait for all specified fences to finish, timeout is disabled

        uint32_t imageIdx;
        result = vkAcquireNextImageKHR(g.device, g.swapchain.swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIdx);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) // e.g. window is resized
        {
            _recreateSwapchainAndFramebuffers(g. physicalDevice, g.device, g.window, g.surface, &g.queueFamilies, &g.swapchain, g.renderPass, g.framebuffers);
            return;
        }
        else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            TI_ASSERT(0, "");
        }

        vkResetFences(g.device, 1, &inFlightFence);
        vkResetCommandBuffer(commandBuffer, 0);
        
        _recordCommandBuffer(commandBuffer, imageIdx, g.framebuffers, &g.swapchain, g.renderPass, g.graphicsPipeline);

        VkSemaphore waitSemaphores[]{ imageAvailableSemaphore };
        VkSemaphore signalSemaphores[]{ renderFinishedSemaphore };
        VkPipelineStageFlags waitStages[]{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = NULL;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VK_CHECK_RESULT(vkQueueSubmit(g.graphicsQueue, 1, &submitInfo, inFlightFence))

        VkSwapchainKHR swapchains[]{ g.swapchain.swapchain };

        VkPresentInfoKHR presentInfo;
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = NULL;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIdx;
        presentInfo.pResults = NULL;

        result = vkQueuePresentKHR(g.presentQueue, &presentInfo);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || g.framebufferResized)
        {
            g.framebufferResized = false;
            _recreateSwapchainAndFramebuffers(g. physicalDevice, g.device, g.window, g.surface, &g.queueFamilies, &g.swapchain, g.renderPass, g.framebuffers);
        }
        else if(result != VK_SUCCESS)
        {
            TI_ASSERT(0, "");
        }

        g.currentFrame = (g.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}