cmake_minimum_required(VERSION 3.20.0)

project(tinker VERSION 0.0.1 LANGUAGES CXX C)

if(CMAKE_SYSTEM MATCHES Windows)
   message(STATUS "Target system is Windows")
elseif(CMAKE_SYSTEM MATCHES Linux)
   message(STATUS "Build host runs Linux")
endif()

add_subdirectory(tinker/)
add_subdirectory(tinkering/vulkan_app/)