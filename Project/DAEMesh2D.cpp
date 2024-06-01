#include "DAEMesh2D.h"
#include <unordered_map>

Mesh2D::~Mesh2D()
{

}

void Mesh2D::draw(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);

	//vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer, m_IndicesCount, 1, 0, 0, 0);
}

void Mesh2D::destory()
{
	vkDestroyBuffer(m_Device, m_IndexBuffer, nullptr);
	vkFreeMemory(m_Device, m_IndexBufferMemory, nullptr);

	vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
	vkFreeMemory(m_Device, m_VertexBufferMemory, nullptr);
}

void Mesh2D::createVertexBuffer(const std::vector<Vertex2D>& vertices, Buffer& buffer, const VkQueue& graphicsQueue, const VkCommandPool& commandPool)
{
	m_VertexCount = static_cast<uint32_t>(vertices.size());
	//assert(m_VertexCount >= 3 && "Vertex2D count must be at least 3");
	VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_Device, stagingBufferMemory);

	buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

	buffer.copyBuffer(stagingBuffer, m_VertexBuffer, bufferSize, graphicsQueue, commandPool);

	vkDestroyBuffer(m_Device, stagingBuffer, nullptr);
	vkFreeMemory(m_Device, stagingBufferMemory, nullptr);
}

void Mesh2D::createIndexBuffer(const std::vector<uint16_t>& indices, Buffer& buffer, const VkQueue& graphicsQueue, const VkCommandPool& commandPool)
{
	m_IndicesCount = static_cast<uint32_t>(indices.size());
	VkDeviceSize bufferSize = sizeof(indices[0]) * m_IndicesCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_Device, stagingBufferMemory);

	buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

	buffer.copyBuffer(stagingBuffer, m_IndexBuffer, bufferSize, graphicsQueue, commandPool);

	vkDestroyBuffer(m_Device, stagingBuffer, nullptr);
	vkFreeMemory(m_Device, stagingBufferMemory, nullptr);
}

