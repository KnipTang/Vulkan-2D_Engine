#pragma once

#include "vulkan/vulkan_core.h"
#include "DAEMesh2D.h"
#include <vector>

class DAEShader2DFill;

class GraphicsPipelineFill
{
public:
	GraphicsPipelineFill() {}
	~GraphicsPipelineFill() = default;

	void createGraphicsPipeline(const VkRenderPass& renderPass, DAEShader2DFill& shader);

	VkPipeline getVkGraphicsPipeline() const {
		return m_GraphicsPipeline;
	}

	void draw(const VkCommandBuffer& commandBuffer, VkExtent2D swapChainExtent, glm::vec3 newColors);

	void addMesh(Mesh2D& mesh) { m_Mesh.emplace_back(&mesh); }

	void initialize(const VkDevice& device);
	void destroy();
private:
	std::vector<Mesh2D*> m_Mesh;

	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;

	VkDevice m_VkDevice;
};
