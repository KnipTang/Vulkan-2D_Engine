#include "DAEBuffer.h"
#include <stdexcept>
#include "Vertex.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

void Buffer::initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkCommandPool& commandPool, const VkQueue& graphicsQueue)
{
	m_VkDevice = device;
	m_VkPhysicalDevice = physicalDevice;
	m_VkCommandPool = commandPool;
	m_VkGraphicsQueue = graphicsQueue;
}

Buffer::Buffer(const VulkanContext& context, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize bufferSize)
{
	m_VkDevice = context.device;
	m_VkPhysicalDevice = context.physicalDevice;
	createBuffer(bufferSize, usage, properties, uniformBuffers, uniformBuffersMemory);
	m_Size = bufferSize;
}

void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	
	if (vkCreateBuffer(m_VkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}
	
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_VkDevice, buffer, &memRequirements);
	
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	
	if (vkAllocateMemory(m_VkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}
	
	vkBindBufferMemory(m_VkDevice, buffer, bufferMemory, 0);
}

void Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkQueue& graphicsQueue, const VkCommandPool& commandPool)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
	/*
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;
	
	VkCommandBuffer commandBuffer{};
	vkAllocateCommandBuffers(m_VkDevice, &allocInfo, &commandBuffer);
	
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	
	vkEndCommandBuffer(commandBuffer);
	
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	
	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);
	
	vkFreeCommandBuffers(m_VkDevice, commandPool, 1, &commandBuffer);
	*/
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_VkPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}


void Buffer::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	//uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	//uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers, uniformBuffersMemory);

		vkMapMemory(m_VkDevice, uniformBuffersMemory, 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
}

void Buffer::upload(const VkDeviceSize& size, void* data)
{
	void* mappedData;
	vkMapMemory(m_VkDevice, uniformBuffersMemory, 0, size, 0, &mappedData);
	memcpy(mappedData, data, static_cast<size_t>(size));
	vkUnmapMemory(m_VkDevice, uniformBuffersMemory);
}

void Buffer::updateUniformBuffer(uint32_t currentImage, VkExtent2D swapChainExtent)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;
	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Buffer::destroy()
{
	//for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		vkDestroyBuffer(m_VkDevice, uniformBuffers, nullptr);
		vkFreeMemory(m_VkDevice, uniformBuffersMemory, nullptr);
	//}
}

VkCommandBuffer Buffer::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_VkCommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_VkDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Buffer::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_VkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_VkGraphicsQueue);

	vkFreeCommandBuffers(m_VkDevice, m_VkCommandPool, 1, &commandBuffer);
}
