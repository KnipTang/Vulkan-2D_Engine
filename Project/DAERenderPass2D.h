#pragma once

#include "vulkan/vulkan_core.h"

class RenderPass2D
{
public:
	RenderPass2D() {}
	~RenderPass2D() = default;

	void createRenderPass(const VkFormat& swapChainImageFormat);

	VkRenderPass getVkRenderPass() const {
		return m_RenderPass;
	}

	void initialize(const VkDevice& device);
	void destroy();
private:
	VkRenderPass m_RenderPass;

	VkDevice m_VkDevice;
};