#include "vulkanbase/VulkanBase.h"
void VulkanBase::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		void* pUser = glfwGetWindowUserPointer(window);
		VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
		vBase->keyEvent(key, scancode, action, mods);
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		void* pUser = glfwGetWindowUserPointer(window);
		VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
		vBase->mouseMove(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		void* pUser = glfwGetWindowUserPointer(window);
		VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
		vBase->mouseEvent(window, button, action, mods);
		});
}

void VulkanBase::keyEvent(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		m_LineWidth+=0.1f;
	}
	if (key == GLFW_KEY_O && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		m_LineWidth -= 0.1f;
	}
	if (key == GLFW_KEY_L && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		m_LineWidthGrid += 0.1f;
	}
	if (key == GLFW_KEY_K && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		m_LineWidthGrid -= 0.1f;
	}
	if (key == GLFW_KEY_G && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		m_ToggleGrid = !m_ToggleGrid;
	}
	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		if (key == GLFW_KEY_1)
		{
			if (shiftPressed)
			{
				m_ColorLine.r -= 0.1f;
			}
			else
			{
				m_ColorLine.r += 0.1f;
			}
		}
		if (key == GLFW_KEY_2)
		{
			if (shiftPressed)
			{
				m_ColorLine.g -= 0.1f;
			}
			else
			{
				m_ColorLine.g += 0.1f;
			}
		}
		if (key == GLFW_KEY_3)
		{
			if (shiftPressed)
			{
				m_ColorLine.b -= 0.1f;
			}
			else
			{
				m_ColorLine.b += 0.1f;
			}
		}

		// Clamp the values to the range [0.0, 1.0] to prevent overflow
		m_ColorLine.r = glm::clamp(m_ColorLine.r, 0.0f, 1.0f);
		m_ColorLine.g = glm::clamp(m_ColorLine.g, 0.0f, 1.0f);
		m_ColorLine.b = glm::clamp(m_ColorLine.b, 0.0f, 1.0f);

		std::cout << "Color Line: " << " { " << m_ColorLine.r << ", " << m_ColorLine.g << ", " << m_ColorLine.b << " }" << '\n';
	}
	if ((key == GLFW_KEY_4 || key == GLFW_KEY_5 || key == GLFW_KEY_6) && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		if (key == GLFW_KEY_4)
		{
			if (shiftPressed)
			{
				m_ColorFill.r -= 0.1f;
			}
			else
			{
				m_ColorFill.r += 0.1f;
			}
		}
		if (key == GLFW_KEY_5)
		{
			if (shiftPressed)
			{
				m_ColorFill.g -= 0.1f;
			}
			else
			{
				m_ColorFill.g += 0.1f;
			}
		}
		if (key == GLFW_KEY_6)
		{
			if (shiftPressed)
			{
				m_ColorFill.b -= 0.1f;
			}
			else
			{
				m_ColorFill.b += 0.1f;
			}
		}

		// Clamp the values to the range [0.0, 1.0] to prevent overflow
		m_ColorFill.r = glm::clamp(m_ColorFill.r, 0.0f, 1.0f);
		m_ColorFill.g = glm::clamp(m_ColorFill.g, 0.0f, 1.0f);
		m_ColorFill.b = glm::clamp(m_ColorFill.b, 0.0f, 1.0f);

		std::cout << "Color Fill: " << " { " << m_ColorFill.r << ", " << m_ColorFill.g << ", " << m_ColorFill.b << " }" << '\n';
	}
	if ((key == GLFW_KEY_7 || key == GLFW_KEY_8 || key == GLFW_KEY_9) && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		if (key == GLFW_KEY_7)
		{
			if (shiftPressed)
			{
				m_ColorGrid.r -= 0.1f;
			}
			else
			{
				m_ColorGrid.r += 0.1f;
			}
		}
		if (key == GLFW_KEY_8)
		{
			if (shiftPressed)
			{
				m_ColorGrid.g -= 0.1f;
			}
			else
			{
				m_ColorGrid.g += 0.1f;
			}
		}
		if (key == GLFW_KEY_9)
		{
			if (shiftPressed)
			{
				m_ColorGrid.b -= 0.1f;
			}
			else
			{
				m_ColorGrid.b += 0.1f;
			}
		}

		// Clamp the values to the range [0.0, 1.0] to prevent overflow
		m_ColorGrid.r = glm::clamp(m_ColorGrid.r, 0.0f, 1.0f);
		m_ColorGrid.g = glm::clamp(m_ColorGrid.g, 0.0f, 1.0f);
		m_ColorGrid.b = glm::clamp(m_ColorGrid.b, 0.0f, 1.0f);

		std::cout << "Color Grid: " << " { " << m_ColorGrid.r << ", " << m_ColorGrid.g << ", " << m_ColorGrid.b << " }" << '\n';
	}
	else
	{
		
	m_Camera.KeyPress(key, scancode, action, mods, elapseTime);
	}


}

void VulkanBase::mouseMove(GLFWwindow* window, double xpos, double ypos)
{
	//int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	//
	//if (state == GLFW_PRESS)
	//{
	//	m_Camera.mouseMove(xpos, ypos, elapseTime);
	//}


	//int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	//if (state == GLFW_PRESS)
	//{
		//float dx = static_cast<float>(xpos) - m_DragStart.x;

		m_Camera.mouseMove(xpos, ypos, m_DragStart, elapseTime);

		//if (dx > 0) {
		//	m_Rotation += 0.01;
		//}
		//else {
		//	m_Rotation -= 0.01;
		//}
	//}
}

void VulkanBase::mouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::cout << "right mouse button pressed\n";
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_DragStart.x = static_cast<float>(xpos);
		m_DragStart.y = static_cast<float>(ypos);

		m_MouseDrawing.ClearMouseClicks();
		m_Mesh2DLineMouse.createVertexBuffer(m_MouseDrawing.CalculateVerInd().vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineMouse.createIndexBuffer(m_MouseDrawing.CalculateVerInd().indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Left mouse button pressed\n";
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float ndcX = static_cast<float>(2.0 * xpos / width - 1.0);
		float ndcY = static_cast<float>(1.0 - 2.0 * ypos / height);

		ndcY *= -1;

		m_MouseDrawing.AddMouseClick(glm::vec2{ ndcX, ndcY });
		m_Mesh2DLineMouse.createVertexBuffer(m_MouseDrawing.CalculateVerInd().vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineMouse.createIndexBuffer(m_MouseDrawing.CalculateVerInd().indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
	}
}