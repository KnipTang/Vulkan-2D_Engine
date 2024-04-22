#include "DAEShader2D.h"
#include <vulkanbase/VulkanUtil.h>
#include <vulkanbase/VulkanBase.h>

#define MAX_FRAMES_IN_FLIGHT 3

void DAEShader2D::initialize(const VkDevice& vkDevice)
{
	m_ShaderStages.push_back(createVertexShaderInfo(vkDevice));
	m_ShaderStages.push_back(createFragmentShaderInfo(vkDevice));
}

void DAEShader2D::DestroyShaderModules(const VkDevice& vkDevice)
{
	for (VkPipelineShaderStageCreateInfo& stageInfo : m_ShaderStages)
	{
		vkDestroyShaderModule(vkDevice, stageInfo.module, nullptr);
	}
	m_ShaderStages.clear();
}

VkPipelineShaderStageCreateInfo DAEShader2D::createFragmentShaderInfo(const VkDevice& vkDevice) {
	std::vector<char> fragShaderCode = readFile(m_FragmentShaderFile);
	VkShaderModule fragShaderModule = createShaderModule(vkDevice, fragShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	return fragShaderStageInfo;
}

VkPipelineShaderStageCreateInfo DAEShader2D::createVertexShaderInfo(const VkDevice& vkDevice) {
	std::vector<char> vertShaderCode = readFile(m_VertexShaderFile);
	VkShaderModule vertShaderModule = createShaderModule(vkDevice, vertShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	return vertShaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo DAEShader2D::createVertexInputStateInfo()
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	m_VertexInputBindingDescription = Vertex2D::getBindingDescription();
	m_VertexInputAttributeDescriptors = Vertex2D::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_VertexInputAttributeDescriptors.size());
	vertexInputInfo.pVertexBindingDescriptions = &m_VertexInputBindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = m_VertexInputAttributeDescriptors.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo DAEShader2D::createInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkShaderModule DAEShader2D::createShaderModule(const VkDevice& vkDevice, const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}
