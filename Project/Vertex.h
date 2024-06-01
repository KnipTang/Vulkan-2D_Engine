#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include "vulkan/vulkan_core.h"
#include <vector>
#define MAX_FRAMES_IN_FLIGHT 3

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct ViewProjection {
	glm::mat4 proj;
	glm::mat4 view;
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Vertex2D {
	glm::vec3 pos;
	glm::vec3 color;
	//glm::vec2 texCoord;

	bool operator==(const Vertex2D& other) const {
		return pos == other.pos && color == other.color/* && texCoord == other.texCoord*/;
	}

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex2D);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex2D, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex2D, color);

		//attributeDescriptions[2].binding = 0;
		//attributeDescriptions[2].location = 2;
		//attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		//attributeDescriptions[2].offset = offsetof(Vertex2D, texCoord);

		return attributeDescriptions;
	}
};

struct VerInd
{
	std::vector<Vertex2D> vertices;
	std::vector<uint16_t> indices;
};

struct Vertex3D {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 normal;

	bool operator==(const Vertex3D& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
	}

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex3D);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex3D, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex3D, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex3D, texCoord);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex3D, normal);

		return attributeDescriptions;
	}
};

namespace std {
	template<> struct hash<Vertex3D> {
		size_t operator()(Vertex3D const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}