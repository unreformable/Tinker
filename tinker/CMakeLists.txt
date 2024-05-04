add_subdirectory(third_party/)

add_library(tinker STATIC
    source/tinker/core/file.cpp
    source/tinker/core/image.cpp
    source/tinker/math/trigonometry.cpp
    source/tinker/math/matrix4.cpp
    source/tinker/math/vector2.cpp
    source/tinker/math/vector3.cpp
    source/tinker/math/vector4.cpp
    source/tinker/platform/platform.cpp
)
target_include_directories(tinker PUBLIC source/)
target_link_libraries(tinker PUBLIC third_party)