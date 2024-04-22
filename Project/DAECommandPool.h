#pragma once
#include "vulkan/vulkan_core.h"
#include "DAECommandBuffer.h"
#include "vulkanbase/VulkanUtil.h"

struct QueueFamilyIndices;
class DAECommandPool {
public:
	DAECommandPool() :
		m_CommandPool{ VK_NULL_HANDLE },
		m_VkDevice{ VK_NULL_HANDLE }
	{
		
	}
	
	~DAECommandPool() = default;

	VkCommandPool getVkCommandPool() const {
		return m_CommandPool;
	}

	void initialize(const VkDevice & device, const QueueFamilyIndices & queue);
	void destroy();

	DAECommandBuffer createCommandBuffer()const;
private:
	VkCommandPool m_CommandPool;
	VkDevice m_VkDevice;
	
};