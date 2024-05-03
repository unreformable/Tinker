add_executable(vulkan_app main.cpp graphics.cpp queue_families.cpp vk_shader_module.cpp swapchain.cpp)
target_link_libraries(vulkan_app PRIVATE tinker)