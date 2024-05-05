#pragma once



namespace ti::graphics
{
    void initalize();
    void terminate();

    bool shouldClose();
    void pollEvents();
    void drawFrame();

    // struct Window
    // {
    //     GLFWwindow *handle;
    //     int width;
    //     int height;
    //     const char *title;
    // };
    // void createWindow(int width, int height, const char *title, Window *window);
    // void destroyWindow(Window *window);

    // struct Swapchain
    // {
    //     Window window;
    //     VkSurfaceKHR surface;
    // };
    // void createSwapchain(const Window *window, Swapchain *swapchain);
    // void destroySwapchain(Swapchain *swapchain);
}