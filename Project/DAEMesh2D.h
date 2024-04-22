#pragma once
#include "vulkan/vulkan_core.h"

#define FLM_FORCE_RADIANS
#define FLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <DAEBuffer.h>
#include <string>

#include "Vertex.h"
class Mesh2D
{
public:
	Mesh2D(VkDevice& device, VkQueue& graphicsQueue, const VkCommandPool& commandPool) : m_Device(device) {};
    ~Mesh2D();

	Mesh2D(const Mesh2D&) = delete;
    void operator=(const Mesh2D&) = delete;

	void draw(VkCommandBuffer commandBuffer);

	void destory();

	void createVertexBuffer(const std::vector<Vertex2D>& vertices, Buffer& buffer, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
	void createIndexBuffer(const std::vector<uint16_t>& indices, Buffer& buffer, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
private:

	VkDevice& m_Device;
	VkBuffer m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
    uint32_t m_VertexCount;
	uint32_t m_IndicesCount;
};
