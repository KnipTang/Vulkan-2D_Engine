#include "DAEMesh3D.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

//Mesh3D::Mesh3D(VkDevice& device, Buffer& buffer, const std::vector<Vertex3D>& vertices, VkQueue& graphicsQueue, const VkCommandPool& commandPool) : m_Device(device), m_Buffer(buffer)
//{
//	//createVertexBuffer(vertices, graphicsQueue, commandPool);
//}

Mesh3D::~Mesh3D()
{

}

void Mesh3D::draw(const VkPipelineLayout& pipelineLayout, const VkCommandBuffer& commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	
	//vkCmdPushConstants(
	//	commandBuffer,
	//	pipelineLayout,
	//	VK_SHADER_STAGE_VERTEX_BIT,
	//	0,
	//	sizeof(UniformBufferObject),
	//	&m_VertexConstant);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_IndicesCount), 1, 0, 0, 0);
}

void Mesh3D::destory()
{
	vkDestroyBuffer(m_Device, m_IndexBuffer, nullptr);
	vkFreeMemory(m_Device, m_IndexBufferMemory, nullptr);

	vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
	vkFreeMemory(m_Device, m_VertexBufferMemory, nullptr);
}

void Mesh3D::createVertexBuffer(const VkQueue& graphicsQueue, Buffer& buffer, const VkCommandPool& commandPool)
{
	m_VertexCount = static_cast<uint32_t>(vertices.size());
	assert(m_VertexCount >= 3 && "Vertex3D count must be at least 3");
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

void Mesh3D::createIndexBuffer(const VkQueue& graphicsQueue,  Buffer& buffer, const VkCommandPool& commandPool)
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

void Mesh3D::loadModel()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex3D, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex3D vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertices.push_back(vertex);
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
