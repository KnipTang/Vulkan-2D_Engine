#include "Scene.h"

#include <glm/vec2.hpp>

#include "vulkanbase/VulkanBase.h"
#include "Vertex.h"
void Scene::DrawScene(VkCommandBuffer commandBuffer, const std::vector<uint16_t> indices)
{
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

VerInd Scene::generateLine(float posX1, float posY1, float posX2, float posY2, glm::vec3 color)
{
	std::vector<Vertex2D> vertices;
	vertices.push_back({ {posX1, posY1}, color });
	vertices.push_back({ {posX2, posY2},color });

	std::vector<uint16_t> indices;
	indices = { 0, 1};

	return VerInd{ vertices, indices };
}

VerInd Scene::generateRectangle(float left, float bottom, float width, float height, glm::vec3 color) {
    std::vector<Vertex2D> vertices;
    vertices.push_back({ {left, bottom}, color });
    vertices.push_back({ {left + width, bottom},color });
    vertices.push_back({ {left + width, bottom + height}, color });
    vertices.push_back({ {left, bottom + height}, color });

	std::vector<uint16_t> indices;
	indices = { 0, 1, 1, 2, 2 , 3, 3, 0 };

    return VerInd{ vertices, indices };
}

/*
std::vector<Vertex2D> Scene::generateRoundTest(float left, float bottom, float width, float height, float cornerRadius) {
	std::vector<Vertex2D> vertices;
	vertices.push_back({ {left + cornerRadius, bottom}, {1.0f, 0.0f, 0.0f} });
	// Top-right corner
	vertices.push_back({ {left + width - cornerRadius, bottom}, {0.0f, 1.0f, 0.0f} });
	// Bottom-right corner
	vertices.push_back({ {left + width - cornerRadius, bottom + height}, {0.0f, 0.0f, 1.0f} });
	// Bottom-left corner
	vertices.push_back({ {left + cornerRadius, bottom + height}, {1.0f, 1.0f, 1.0f} });
	return vertices;
}

std::vector<uint16_t> Scene::generateIndicesRoundTest() {
	return {
		// Rectangle
		0, 1, 2, 2, 3, 0,
		// Rounded corners (example indices; adjust based on your vertex arrangement)
		4, 5, 6, 6, 7, 4, // Top-left corner
		8, 9, 10, 10, 11, 8, // Top-right corner
		12, 13, 14, 14, 15, 12, // Bottom-right corner
		16, 17, 18, 18, 19, 16 // Bottom-left corner
	};
}
*/

VerInd Scene::generateOval(float x, float y, float radius, int numSegments, glm::vec3 color) {
    std::vector<Vertex2D> vertices;
    for (int i = 0; i < numSegments; i++)
    {
        float angle = static_cast<float>(2 * PI * i / numSegments);
        glm::vec2 point(x + radius * glm::cos(angle), y + radius * glm::sin(angle));
        vertices.push_back({ point, color });
    }

    vertices.push_back({ {x, y}, color });

	std::vector<uint16_t> indices;
	for (int i = 0; i < numSegments - 1; ++i) {
		indices.push_back(i);
		indices.push_back(i + 1);
		//indices.push_back(numSegments);
	}
	// Connect the last segment to the first
	indices.push_back(numSegments - 1);
	indices.push_back(0);
	//indices.push_back(numSegments);

	return VerInd{ vertices, indices };
}

VerInd Scene::generateRoundedRectangle(float posX, float posY, float width, float height, int numSegments, glm::vec3 color) 
{
	glm::vec2 pos = { posX , posY };

	float radians = PI / 2 / numSegments;
	//width /= 2;
	//height /= 2;

	std::vector<Vertex2D> finalVertexes;

	// Upper Rect
	Vertex2D vertices = { glm::vec2{pos.x + width/2, pos.y - height/2}, color };

	//finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	//finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	//finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	//finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	std::vector<uint16_t> indices =
	{
	   //0, 1, 2, 2, 3, 0
	};

	Vertex2D startPoint{ {pos.x, pos.y },vertices.color };
	Vertex2D endPoint{};


	finalVertexes.push_back({ startPoint.pos, color });



	//1
	startPoint.pos = { vertices.pos.x ,vertices.pos.y + height };

	startPoint.color = glm::vec3{ 1.0f, 0.f, 0.f };
	endPoint.color = glm::vec3{ 1.0f, 0.f, 0.f };

	finalVertexes.push_back({ startPoint.pos, color }); //0
	size_t size = finalVertexes.size(); 

	for (int i = 0; i <= numSegments; i++)
	{
		float angle = radians * i;
		glm::vec2 point(startPoint.pos.x + width * glm::cos(angle), startPoint.pos.y + height * glm::sin(angle));
		finalVertexes.push_back({ point, color }); //4
	}
	for (int i = 0; i <= numSegments - 1; i++)
	{
		indices.push_back(static_cast<uint16_t>(i + size));
		indices.push_back(static_cast<uint16_t>(i + size) + 1);
		//indices.push_back(static_cast<uint16_t>(0));
	}


	//2
	startPoint.pos = { vertices.pos.x - width,vertices.pos.y + height };

	finalVertexes.push_back({ startPoint.pos, color }); // 4
	size = finalVertexes.size();

	for (int i = 0; i <= numSegments; i++)
	{
		float angle = PI / 2 + radians * i;
		glm::vec2 point(startPoint.pos.x + width * glm::cos(angle), startPoint.pos.y + height * glm::sin(angle));
		finalVertexes.push_back({ point, color }); //5,6,7
	}
	indices.push_back(static_cast<uint16_t>(size-2));
	indices.push_back(static_cast<uint16_t>(size));
	for (int i = 0; i <= numSegments - 1; i++)
	{
		indices.push_back(static_cast<uint16_t>(i + size));
		indices.push_back(static_cast<uint16_t>((i + size - 3) % (numSegments + size + 1) + 4)); // cyclically connect vertices
		//indices.push_back(static_cast<uint16_t>(0)); // index of the center vertex
	}


	//3
	startPoint.pos = { vertices.pos.x - width,vertices.pos.y };

	finalVertexes.push_back({ startPoint.pos, color });
	size = finalVertexes.size();

	for (int i = 0; i <= numSegments; i++)
	{
		float angle = PI + radians * i;
		glm::vec2 point(startPoint.pos.x + width * glm::cos(angle), startPoint.pos.y + height * glm::sin(angle));
		finalVertexes.push_back({ point, color });
	}
	indices.push_back(static_cast<uint16_t>(size - 2));
	indices.push_back(static_cast<uint16_t>(size));
	for (int i = 0; i <= numSegments - 1; i++)
	{
		indices.push_back(static_cast<uint16_t>(i + size)); // index starts from the last vertex of the previous loop
		indices.push_back(static_cast<uint16_t>((i + size - 3) % (numSegments + size + 1) + 4)); // cyclically connect vertices
		//indices.push_back(static_cast<uint16_t>(0)); // index of the center vertex
	}


	//4
	startPoint.pos = { vertices.pos.x,vertices.pos.y };

	finalVertexes.push_back({ startPoint.pos, color });
	size = finalVertexes.size();

	for (int i = 0; i <= numSegments; i++)
	{
		float angle = 3 * PI / 2 + radians * i;
		glm::vec2 point(startPoint.pos.x + width * glm::cos(angle), startPoint.pos.y + height * glm::sin(angle));
		finalVertexes.push_back({ point, color });
	}
	indices.push_back(static_cast<uint16_t>(size - 2));
	indices.push_back(static_cast<uint16_t>(size));
	for (int i = 0; i <= numSegments - 1; i++)
	{
		indices.push_back(static_cast<uint16_t>(i + size)); // index starts from the last vertex of the previous loop
		indices.push_back(static_cast<uint16_t>((i + size - 3) % (numSegments + size + 1) + 4)); // cyclically connect vertices
		//indices.push_back(static_cast<uint16_t>(0)); // index of the center vertex
	}



	indices.push_back(static_cast<uint16_t>(finalVertexes.size() - 1));
	indices.push_back(static_cast<uint16_t>(2));



	VerInd verInd = VerInd{ finalVertexes,indices };
    return verInd;
}

VerInd Scene::generateArc(float x, float y, float radius, int numSegments, float arcAngleInDegrees, glm::vec3 color)
{
	std::vector<Vertex2D> vertices;
	// Convert arc angle from degrees to radians
	float arcAngle = glm::radians(arcAngleInDegrees);
	// Calculate the angle increment based on the arc angle
	float angleIncrement = arcAngle / (numSegments - 1);
	for (int i = 0; i < numSegments; i++)
	{
		float angle = static_cast<float>(angleIncrement * i);
		glm::vec2 point(x + radius * glm::cos(angle), y + radius * glm::sin(angle));
		vertices.push_back({ point, color });
	}
	vertices.push_back({ {x, y}, color });


	std::vector<uint16_t> indices;
	for (int i = 0; i < numSegments - 1; ++i) {
		indices.push_back(i);
		indices.push_back(i + 1);
		//indices.push_back(numSegments);
	}
	// Connect the last segment to the first
	indices.push_back(numSegments - 1);
	indices.push_back(numSegments);
	indices.push_back(numSegments);
	indices.push_back(0);
	//indices.push_back(numSegments);

	return VerInd{ vertices, indices };
}

VerInd Scene::generateSpiral(float x, float y, float majorRadius, float minorRadius, int numMajorSegments, int numMinorSegments, glm::vec3 color)
{
	std::vector<Vertex2D> vertices;
	std::vector<uint16_t> indices;

	for (int i = 0; i < numMajorSegments; ++i) {
		float majorAngle = static_cast<float>(2 * PI * i / numMajorSegments);
		glm::vec3 majorCirclePoint(x + majorRadius * glm::cos(majorAngle), y + majorRadius * glm::sin(majorAngle), 0.0f);

		for (int j = 0; j < numMinorSegments; ++j) {
			float minorAngle = static_cast<float>(2 * PI * j / numMinorSegments);
			glm::vec3 minorCircleOffset(minorRadius * glm::cos(minorAngle), minorRadius * glm::sin(minorAngle), 0.0f);

			glm::vec3 point = majorCirclePoint + minorCircleOffset;

			vertices.push_back({ point, color /*glm::normalize(point - majorCirclePoint)*/ }); // Normal pointing from center of torus to surface
		}
	}

	// Generate indices
	for (int i = 0; i < numMajorSegments; ++i) {
		int currentIndex = i * numMinorSegments;
		int nextIndex = (i + 1) % numMajorSegments * numMinorSegments;
		for (int j = 0; j < numMinorSegments; ++j) {
			indices.push_back(currentIndex + j);
			indices.push_back(nextIndex + j);
			indices.push_back(nextIndex + (j + 1) % numMinorSegments);

			indices.push_back(currentIndex + j);
			indices.push_back(nextIndex + (j + 1) % numMinorSegments);
			indices.push_back(currentIndex + (j + 1) % numMinorSegments);
		}
	}

	return VerInd{ vertices, indices };
}

VerInd Scene::generateDonut(float x, float y, float outerRadius, float innerRadius, int numSegments, glm::vec3 color)
{
	std::vector<Vertex2D> vertices;
	std::vector<uint16_t> indices;

	// Generate outer circle vertices
	for (int i = 0; i < numSegments; ++i) {
		float angle = static_cast<float>(2 * PI * i / numSegments);
		glm::vec2 outerPoint(x + outerRadius * glm::cos(angle), y + outerRadius * glm::sin(angle));
		vertices.push_back({ outerPoint, color });
	}

	// Generate inner circle vertices
	for (int i = 0; i < numSegments; ++i) {
		float angle = static_cast<float>(2 * PI * i / numSegments);
		glm::vec2 innerPoint(x + innerRadius * glm::cos(angle), y + innerRadius * glm::sin(angle));
		vertices.push_back({ innerPoint, color });
	}

	// Generate indices
	for (int i = 0; i < numSegments; ++i) {
		int currentIndex = i;
		int nextIndex = (i + 1) % numSegments;

		// Outer circle
		indices.push_back(currentIndex);
		indices.push_back(nextIndex);
		//indices.push_back(currentIndex + numSegments);

		//indices.push_back(nextIndex);
		indices.push_back(nextIndex + numSegments);
		indices.push_back(currentIndex + numSegments);
	}

	return VerInd{ vertices, indices };
}