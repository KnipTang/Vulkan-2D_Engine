#pragma once

#include "vulkan/vulkan_core.h"
#include "DAEDepth.h"

class DAECommandBuffer;
class RenderPass
{
public:
	RenderPass() {}
	~RenderPass() = default;

	void createRenderPass(const VkFormat& swapChainImageFormat, Depth depth);

	VkRenderPass getVkRenderPass() const{
		return m_RenderPass;
	}

	void beginRenderPass(const DAECommandBuffer& buffer, VkFramebuffer currentBuffer, VkExtent2D extent);
	void endRenderPass(const DAECommandBuffer& buffer);

	void initialize(const VkDevice& device);
	void destroy();
private:
	VkRenderPass m_RenderPass;

	VkDevice m_VkDevice;
};