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
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_REPEAT)
	{
		std::cout << "right mouse button pressed\n";
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_DragStart.x = static_cast<float>(xpos);
		m_DragStart.y = static_cast<float>(ypos);
	}
}