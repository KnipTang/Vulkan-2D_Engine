#pragma once
#include <memory>
#include <vector>
#include "DAEShader3D.h"
#include "DAEGraphicsPipeline.h"
#include "DAEMesh3D.h"
#include "DAETexture.h"
#include "DAEBuffer.h"
#include "vulkan/vulkan_core.h"
#include "DAEDescriptors.h"

#include "Vertex.h"
class GraphicsPipeline3D : public GraphicsPipeline
{
public:
	GraphicsPipeline3D() { }
	~GraphicsPipeline3D() = default;

	void createGraphicsPipeline(const VkRenderPass& renderPass, DAEShader3D& shader);

	void virtual DrawFrame(const VkCommandBuffer& commandBuffer, VkExtent2D swapChainExtent);

	void addMesh(Mesh3D& mesh) { m_Mesh.emplace_back(&mesh); }

	void setUBO(ViewProjection ubo, size_t uboIndex);

	void destroy()
	{
		vkDestroyPipeline(m_VkDevice, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_VkDevice, m_PipelineLayout, nullptr);

		m_UBOPool.get()->destroy();
		//m_Texture->destroy();
		//delete m_Buffer;
		//delete m_Texture;

		//
		//// Clear the vector
		//m_Mesh.clear();
	}

	void initialize(const VulkanContext& context, Buffer& buffer, Texture& texture);
private:
	std::vector<Mesh3D*> m_Mesh;

	std::unique_ptr<Descriptors<ViewProjection>> m_UBOPool;
};
