add_subdirectory(glad-4.5/)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.4/)

set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glm-1.0.0/)

add_subdirectory(stb/)

#find_package(Vulkan REQUIRED)
add_subdirectory(Vulkan-Headers-1.3.280/)
add_subdirectory(Vulkan-Loader-1.3.280/)



add_library(third_party STATIC empty.cpp)
target_include_directories(third_party PUBLIC Vulkan-Headers-1.3.280/include/) # ${Vulkan_INCLUDE_DIRS}
target_link_libraries(third_party PUBLIC glad-4.5 glfw glm stb vulkan) # Vulkan::Headers ${Vulkan_LIBRARIES}


# set(VULKAN_HEADERS
#     # vk_video/
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_av1std_decode.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_av1std.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_h264std_decode.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_h264std_encode.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_h264std.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_h265std_decode.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_h265std_encode.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codec_h265std.h
#     Vulkan-Headers-1.3.280/include/vk_video/vulkan_video_codecs_common.h

#     # vulkan/
#     Vulkan-Headers-1.3.280/include/vulkan/vk_icd.h
#     Vulkan-Headers-1.3.280/include/vulkan/vk_layer.h
#     Vulkan-Headers-1.3.280/include/vulkan/vk_platform.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan.cppm
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_android.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_beta.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_core.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_directfb.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_enums.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_extension_inspection.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_format_traits.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_fuchsia.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_funcs.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_ggp.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_handles.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_hash.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_hpp_macros.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_ios.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_macos.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_metal.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_raii.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_screen.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_shared.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_static_assertions.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_structs.hpp
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_to_string.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_vi.h
#     #Vulkan-Headers-1.3.280/include/vulkan/vulkan_video.hpp
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_wayland.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_win32.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_xcb.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_xlib.h
#     Vulkan-Headers-1.3.280/include/vulkan/vulkan_xlib_xrandr.h
# )

# set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
# add_subdirectory(Vulkan-Headers-1.3.280/)
# add_subdirectory(Vulkan-Loader-1.3.280/)