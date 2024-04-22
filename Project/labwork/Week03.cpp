#include "vulkanbase/VulkanBase.h"

void VulkanBase::createFrameBuffers() {
	swapChainFramebuffers.resize(m_SwapChain.getVkSwapChainImageViews().size());
	for (size_t i = 0; i < m_SwapChain.getVkSwapChainImageViews().size(); i++) {
		std::array<VkImageView, 2> attachments = {
			m_SwapChain.getVkSwapChainImageViews()[i],
			m_Depth.getVkDepthImageView()
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_RenderPass.getVkRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = m_SwapChain.getVkSwapChainExtent().width;
		framebufferInfo.height = m_SwapChain.getVkSwapChainExtent().height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}