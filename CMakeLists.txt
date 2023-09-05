cmake_minimum_required(VERSION 3.20)
project(InterOP)

set(CMAKE_CXX_STANDARD 20)

set(VULKAN_SDK "C:/VulkanSDK/1.3.239.0")

file(GLOB SOURCE_FILE
    "*.*"
)

include_directories(${VULKAN_SDK}/include)

add_executable(InterOP ${SOURCE_FILE})

target_link_libraries(InterOP PUBLIC
    ${VULKAN_SDK}/lib/vulkan-1.lib
)