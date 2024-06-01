#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"

class MouseDrawing {
private:

public:
    MouseDrawing() {};
    ~MouseDrawing() = default;

    void AddMouseClick(glm::vec2 pos, float mouseDepth) 
    { 
        m_MouseClicksPos.emplace_back(pos, mouseDepth);
        std::cout << "Added Mouse Click: " << pos.x << ", " << pos.y << '\n';

        CalculateVerInd();
    }
    void ClearMouseClicks() 
    { 
        m_MouseClicksPos.clear(); 
        std::cout << "Cleared Mouse" << '\n';
    }

    VerInd CalculateVerInd()
    {

        CalculateVertices();
        CalculateIndices();

        return VerInd{ m_Vertices, m_Indices };
    }

    std::vector<glm::vec3> GetMouseClicks() { return m_MouseClicksPos; }

private:

    void CalculateVertices()
    {
        m_Vertices.clear();

        if (m_MouseClicksPos.empty()) {
            m_Vertices.push_back(Vertex2D{ glm::vec3{-2.0f, -2.0f, 0}, glm::vec3{1.0f, 1.0f, 1.0f} });
            return;
        }

        for (size_t i = 0; i < m_MouseClicksPos.size(); i++)
        {
            m_Vertices.push_back(Vertex2D{ m_MouseClicksPos.at(i), glm::vec3{0.0f, 0.0f, 0.0f} });
        }
    }

    void CalculateIndices()
    {
        m_Indices.clear();

        if (m_Vertices.size() <= 1) {
            m_Indices.emplace_back(0);
            m_Indices.emplace_back(0);
            return;
        }

        m_Indices.emplace_back(0);
        for (size_t i = 1; i < m_Vertices.size() - 1; i++)
        {
            m_Indices.emplace_back(i);
            m_Indices.emplace_back(i);
        }
        m_Indices.emplace_back(m_Vertices.size() - 1);
    }

    std::vector<glm::vec3> m_MouseClicksPos;

    std::vector<Vertex2D> m_Vertices;
    std::vector<uint16_t> m_Indices;

    MouseDrawing(const MouseDrawing&) = delete;
    MouseDrawing& operator=(const MouseDrawing&) = delete;
    MouseDrawing(const MouseDrawing&&) = delete;
    MouseDrawing& operator=(const MouseDrawing&&) = delete;
};