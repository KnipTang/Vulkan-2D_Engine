# Source files
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

set(TEXTURES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/textures")
set(TEXTURES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/textures")
file(MAKE_DIRECTORY "${TEXTURES_OUTPUT_DIR}")

set(MODELS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/models")
set(MODELS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/models")
file(MAKE_DIRECTORY "${MODELS_OUTPUT_DIR}")

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)


set(SOURCES
    "main.cpp"
    "vulkanbase/VulkanBase.h" 
    "vulkanbase/VulkanUtil.h"
    # Add other source files here
    "labwork/Week01.cpp"
    "labwork/Week02.cpp" 
    "labwork/Week03.cpp" 
    "labwork/Week04.cpp" 
    "labwork/Week05.cpp"
    "labwork/Week06.cpp"
)


# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${GLSL_SOURCE_FILES}   "2D/DAEGraphicsPipelineFill.cpp" "2D/DAEGraphicsPipelineFill.h" "2D/DAEGraphicsPipelineTemp.cpp" "2D/DAEGraphicsPipelineTemp.h"  "DemoCode/DAEUniformBufferObject.h"  "vulkanbase/VulkanUtil.cpp" "labwork/Week03.cpp" "DAEShader3D.cpp" "DAEShader3D.h" "DAEShader2D.cpp" "DAEShader2D.h" "DAECommandPool.h" "DAECommandPool.cpp" "DAECommandBuffer.h" "DAECommandBuffer.cpp" "Scene.h" "Scene.cpp" "DAERenderPass.h" "DAERenderPass.cpp" "DAEGraphicsPipeline.h" "DAEGraphicsPipeline.cpp" "DAEBuffer.cpp" "DAEBuffer.h" "DAEMesh3D.h" "DAEMesh3D.cpp" "DAEMesh2D.h" "DAEMesh2D.cpp" "Vertex.h" "DAEDescriptors.h" "DAEDescriptors.cpp" "DAEDepth.h" "DAEDepth.cpp" "DAETexture.h" "DAETexture.cpp" "DAESwapChain.h" "DAESwapChain.cpp" "DAEDevice.h" "DAEDevice.cpp"   "DAEGraphicsPipeline3D.h" "DAEGraphicsPipeline3D.cpp" "DAECamera.h" "DAECamera.cpp" "DAERenderPass2D.h" "DAERenderPass2D.cpp" "DAEShader2DFill.h" "DAEShader2DFill.cpp" "SceneFill.h" "SceneFill.cpp")
add_dependencies(${PROJECT_NAME} Shaders)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${TEXTURES_SOURCE_DIR}" "${TEXTURES_OUTPUT_DIR}"
    COMMENT "Copying textures to output directory"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${MODELS_SOURCE_DIR}" "${MODELS_OUTPUT_DIR}"
    COMMENT "Copying models to output directory"
)

# Link libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} tinyobjloader)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} stb)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} stb)