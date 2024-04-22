cmake_minimum_required(VERSION 3.10)

# Project Name
project(VulkanLab01)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8 # Specify the version you need
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(tinyobjloader
  GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
  GIT_TAG v1.0.6
  #GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tinyobjloader)


FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG        master  # Optionally specify a commit hash or tag to download a specific version
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
)

FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
  FetchContent_Populate(stb)
  include_directories(${stb_SOURCE_DIR})  # Include the directory where the headers are downloaded
endif()

# Find the required packages
find_package(Vulkan REQUIRED)

# Include Directories
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(Project)

# If using validation layers, copy the required JSON files (optional)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_SOURCE_DIR}/path_to_json/VK_LAYER_KHRONOS_validation.json"
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
