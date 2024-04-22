#pragma once
#include <vector>
#include "DAEMesh3D.h"
#include "vulkan/vulkan_core.h"

class Scene
{
public:
	Scene() {};
	~Scene() = default;

	void DrawScene(VkCommandBuffer commandBuffer, const std::vector<uint16_t> indices);

	static std::vector<Vertex2D> generateRectangle(float top, float left, float bottom, float right);
	static std::vector<uint16_t> generateIndicesForRectangle();

	static std::vector<Vertex2D> generateOval(float x, float y, float radius, int numSegments);
	static std::vector<uint16_t> generateIndicesForOval(int numSegments);

	//static std::vector<Vertex3D> generateRoundedRectangle(float top, float left, float bottom, float right, float radius, int numSegments);
	//static std::vector<uint16_t> generateIndicesForRoundedRectangle(int numSegments);
private:
};
