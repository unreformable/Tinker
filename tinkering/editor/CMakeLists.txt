add_executable(editor main.cpp graphics.cpp queue_families.cpp vk_shader_module.cpp swapchain.cpp)
target_link_libraries(editor PRIVATE tinker)