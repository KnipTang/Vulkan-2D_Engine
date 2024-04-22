#include "DAEGraphicsPipeline.h"
#include "DAEDescriptors.h"
#include <stdexcept>
#include <vector>
#include "Vertex.h"

void GraphicsPipeline::initialize(const VulkanContext& context)
{
	//m_UBOPool = std::make_unique<DAEDescriptorPool<ViewProjection>(context.device, 1);
	//m_UBOPool->initialize(context);
}

void GraphicsPipeline::createGraphicsPipeline(const VkRenderPass& renderPass)
{

}

void GraphicsPipeline::destroy()
{
	vkDestroyPipeline(m_VkDevice, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_VkDevice, m_PipelineLayout, nullptr);
}

void GraphicsPipeline::setUBO(ViewProjection ubo, size_t uboIndex)
{

}

void GraphicsPipeline::DrawFrame(const VkCommandBuffer& commandBuffer, VkExtent2D swapChainExtent)
{

}