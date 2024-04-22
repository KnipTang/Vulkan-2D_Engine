#pragma once
#include <string>
#include <vector>
#include <array>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

class DAEShader3D {
private:
    GLFWwindow* window;

public:
    DAEShader3D(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
        : m_VertexShaderFile{ vertexShaderFile }, m_FragmentShaderFile{ fragmentShaderFile }
    {
    };
    ~DAEShader3D() = default;
    std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() { return m_ShaderStages; };
    void initialize(const VkDevice& vkDevice);
    void DestroyShaderModules(const VkDevice& vkDevice);

    VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo();
    VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();

private:
    VkPipelineShaderStageCreateInfo createFragmentShaderInfo(const VkDevice& vkDevice);
    VkPipelineShaderStageCreateInfo createVertexShaderInfo(const VkDevice& vkDevice); //creates the information about the vertex lay?out that we want to use in combination with the shader.
    static VkShaderModule createShaderModule(const VkDevice& vkDevice, const std::vector<char>& code); //Can be made static because it doesnt depend on anything else from the class

    std::string m_VertexShaderFile;
    std::string m_FragmentShaderFile;

    std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

    VkVertexInputBindingDescription m_VertexInputBindingDescription;
    std::vector<VkVertexInputAttributeDescription> m_VertexInputAttributeDescriptors;

    DAEShader3D(const DAEShader3D&) = delete;
    DAEShader3D& operator=(const DAEShader3D&) = delete;
    DAEShader3D(const DAEShader3D&&) = delete;
    DAEShader3D& operator=(const DAEShader3D&&) = delete;
};