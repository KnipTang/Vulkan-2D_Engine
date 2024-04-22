#pragma once
#include "vulkan/vulkan_core.h"

#define FLM_FORCE_RADIANS
#define FLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <DAEBuffer.h>
#include <string>

#include "Vertex.h"
class Mesh3D
{
public:

	//Mesh3D(VkDevice& device, Buffer& buffer, const std::vector<Vertex3D>& vertices, VkQueue& graphicsQueue, const VkCommandPool& commandPool);
	Mesh3D(VkDevice& device, VkQueue& graphicsQueue, const VkCommandPool& commandPool, std::string modelPath) : m_Device(device), MODEL_PATH(modelPath){};
    ~Mesh3D();

	//Mesh3D(const Mesh3D&) = delete;
   // void operator=(const Mesh3D&) = delete;

	void draw(const VkPipelineLayout& pipelineLayout, const VkCommandBuffer& commandBuffer);

	void destory();

    //void createVertexBuffer(const std::vector<Vertex3D>& vertices, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
	//void createIndexBuffer(const std::vector<uint16_t>& indices, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);

	void createVertexBuffer(const VkQueue& graphicsQueue, Buffer& buffer, const VkCommandPool& commandPool);
	void createIndexBuffer(const VkQueue& graphicsQueue,  Buffer& buffer, const VkCommandPool& commandPool);

	void loadModel();
private:

	const std::string MODEL_PATH = "models/viking_room.obj";

	VkDevice& m_Device;
	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;
	VkBuffer m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
    uint32_t m_VertexCount;
	uint32_t m_IndicesCount;
};
