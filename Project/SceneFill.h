#pragma once
#include <vector>
#include "DAEMesh3D.h"
#include "vulkan/vulkan_core.h"

class SceneFill
{
public:
	SceneFill() {};
	~SceneFill() = default;

	void DrawScene(VkCommandBuffer commandBuffer, const std::vector<uint16_t> indices);

	static VerInd generateRectangle(float left, float bottom, float width, float height, float depth = 0, glm::vec3 color = { 1,1,1 });

	static VerInd generateOval(float x, float y, float radius, int numSegments, float depth = 0, glm::vec3 color = { 1,1,1 });

	static VerInd generateRoundedRectangle(float posX, float posY, float width, float height, int numSegments, float depth = 0, glm::vec3 color = { 1,1,1 });

	static VerInd generateArc(float x, float y, float radius, int numSegments, float arcAngleInDegrees, float depth = 0, glm::vec3 color = { 1,1,1 });

	static VerInd generateSpiral(float x, float y, float majorRadius, float minorRadius, int numMajorSegments, int numMinorSegments, float depth = 0, glm::vec3 color = { 1,1,1 });

	static VerInd generateDonut(float x, float y, float outerRadius, float innerRadius, int numSegments, float depth = 0, glm::vec3 color = { 1,1,1 });

private:
};
