#include "Scene.h"

#include <glm/vec2.hpp>

#include "vulkanbase/VulkanBase.h"
#include "Vertex.h"
void Scene::DrawScene(VkCommandBuffer commandBuffer, const std::vector<uint16_t> indices)
{
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

std::vector<Vertex2D> Scene::generateRectangle(float top, float left, float bottom, float right) {
    std::vector<Vertex2D> vertices;
    vertices.push_back({ {left, top}, {1.0f, 0.0f, 0.0f} });    
    vertices.push_back({ {right, top}, {0.0f, 1.0f, 0.0f} });
    vertices.push_back({ {right, bottom}, {0.0f, 0.0f, 1.0f} });
    vertices.push_back({ {left, bottom}, {1.0f, 1.0f, 1.0f} });
    return vertices;
}

std::vector<uint16_t> Scene::generateIndicesForRectangle() {
    return { 0, 1, 2, 2, 3, 0 };
}

std::vector<Vertex2D> Scene::generateOval(float x, float y, float radius, int numSegments) {
    std::vector<Vertex2D> vertices;
    float theta = 2 * 3.1415926f / numSegments;
    for (int i = 0; i < numSegments; ++i) {
        float angle = theta * i;
        float vx = x + radius * std::cos(angle);
        float vy = y + radius * std::sin(angle);
        vertices.push_back({ {vx, vy}, {1.0f, 0.0f, 0.0f} });
    }
    return vertices;
}

std::vector<uint16_t> Scene::generateIndicesForOval(int numSegments) {
    std::vector<uint16_t> indices;
    for (int i = 1; i < numSegments - 1; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(0);
    indices.push_back(numSegments - 1);
    indices.push_back(1);
    return indices;
}

//std::vector<Vertex2D> Scene::generateRoundedRectangle(float top, float left, float bottom, float right, float radius, int numSegments) {
//    std::vector<Vertex2D> vertices;
//
//    // Calculate positions of the corners
//    float topLeft[2] = { left + radius, top - radius };
//    float topRight[2] = { right - radius, top - radius };
//    float bottomRight[2] = { right - radius, bottom + radius };
//    float bottomLeft[2] = { left + radius, bottom + radius };
//
//    // Generate m_Vertices for the rounded edges
//    for (int i = 0; i <= numSegments / 4; ++i) {
//        float angle = (i * 3.1415926f / (2 * numSegments));
//
//        // Top-left corner
//        float vx = topLeft[0] + radius * std::cos(angle);
//        float vy = topLeft[1] + radius * std::sin(angle);
//        vertices.push_back({ {vx, vy, 0.0f}, {1.0f, 1.0f, 1.0f} });
//
//        // Top-right corner
//        vx = topRight[0] + radius * std::cos(angle + 3.1415926f / 2.0f);
//        vy = topRight[1] + radius * std::sin(angle + 3.1415926f / 2.0f);
//        vertices.push_back({ {vx, vy, 0.0f}, {1.0f, 1.0f, 1.0f} });
//
//        // Bottom-right corner
//        vx = bottomRight[0] + radius * std::cos(angle + 3.1415926f);
//        vy = bottomRight[1] + radius * std::sin(angle + 3.1415926f);
//        vertices.push_back({ {vx, vy, 0.0f}, {1.0f, 1.0f, 1.0f} });
//
//        // Bottom-left corner
//        vx = bottomLeft[0] + radius * std::cos(angle + 3.1415926f / 2.0f);
//        vy = bottomLeft[1] + radius * std::sin(angle + 3.1415926f / 2.0f);
//        vertices.push_back({ {vx, vy, 0.0f}, {1.0f, 1.0f, 1.0f} });
//    }
//
//    return vertices;
//}
//
//std::vector<uint16_t> Scene::generateIndicesForRoundedRectangle(int numSegments) {
//    std::vector<uint16_t> indices;
//    int numVertices = numSegments / 4 + 1;
//    for (int i = 0; i < numVertices - 1; ++i) {
//        indices.push_back(i);
//        indices.push_back(i + 1);
//        indices.push_back(numVertices + i);
//        indices.push_back(numVertices + i);
//        indices.push_back(numVertices + i + 1);
//        indices.push_back(i + 1);
//    }
//    indices.push_back(numVertices - 1);
//    indices.push_back(0);
//    indices.push_back(2 * numVertices - 1);
//    indices.push_back(2 * numVertices - 1);
//    indices.push_back(numVertices);
//    indices.push_back(0);
//    return indices;
//}
//