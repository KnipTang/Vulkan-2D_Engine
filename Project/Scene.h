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

	static std::vector<Vertex2D> generateRectangle(float left, float bottom, float width, float height, glm::vec3 color = {1,1,1});
	static std::vector<uint16_t> generateIndicesForRectangle();

	static VerInd generateOval(float x, float y, float radius, int numSegments, glm::vec3 color = { 1,1,1 });

	static VerInd generateRoundedRectangle(float left, float bottom, float width, float height, float radius, int numSegments, glm::vec3 color = { 1,1,1 });

	static VerInd generateArc(float x, float y, float radius, int numSegments, float arcAngleInDegrees, glm::vec3 color = { 1,1,1 });

	static VerInd generateSpiral(float x, float y, float majorRadius, float minorRadius, int numMajorSegments, int numMinorSegments, glm::vec3 color = { 1,1,1 });

	static VerInd generateDonut(float x, float y, float outerRadius, float innerRadius, int numSegments, glm::vec3 color = { 1,1,1 });

private:
};
