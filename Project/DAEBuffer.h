#pragma once
#include <vector>
#include "vulkan/vulkan_core.h"
#include "vulkanbase/VulkanUtil.h"
#include <iostream>
struct ViewProjection;
class Buffer
{
public:
	Buffer() = default;
	Buffer(const VulkanContext& context, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize bufferSize);
	~Buffer() {
		//destroy();
	};

	//void createVertexBuffer(const std::vector<Vertex3D> &vertices);
	//void createIndexBuffer();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
	
	void initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkCommandPool& commandPool, const VkQueue& graphicsQueue);

	void createUniformBuffers();

	void upload(const VkDeviceSize& size, void* data);

	void updateUniformBuffer(uint32_t currentImage, VkExtent2D swapChainExtent);

	const VkBuffer& getUniformBuffer()
	{
		return uniformBuffers;
	}
	std::vector<void*> getUniformBuffersMapped()
	{
		return uniformBuffersMapped;
	}

	void destroy();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
private:

	VkDevice m_VkDevice;
	VkPhysicalDevice m_VkPhysicalDevice;
	VkCommandPool m_VkCommandPool;
	VkQueue m_VkGraphicsQueue;

	VkBuffer uniformBuffers;
	VkDeviceMemory uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDeviceSize m_Size;
};