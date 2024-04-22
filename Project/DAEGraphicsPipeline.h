#pragma once

#include <memory>

#include "Vertex.h"
#include "vulkan/vulkan_core.h"

#include "DAEDescriptors.h"

class DAEShader3D;

class GraphicsPipeline
{
public:
	GraphicsPipeline() {}
	~GraphicsPipeline() = default;

	void virtual createGraphicsPipeline(const VkRenderPass& renderPass);

	VkPipeline getVkGraphicsPipeline() const {
		return m_GraphicsPipeline;
	}
	VkPipelineLayout getVkPipelineLayout() const {
		return m_PipelineLayout;
	}

	void virtual DrawFrame(const VkCommandBuffer& commandBuffer, VkExtent2D swapChainExtent);

	void virtual initialize(const VulkanContext& context);
	void virtual destroy();

	void virtual setUBO(ViewProjection ubo, size_t uboIndex);

	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	VkDevice m_VkDevice;
	int currentFrame = 0;
private:
};
