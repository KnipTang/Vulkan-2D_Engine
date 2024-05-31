#include "vulkanbase/VulkanBase.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
void VulkanBase::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


void VulkanBase::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void VulkanBase::createSyncObjects() {
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}

}

void VulkanBase::drawFrame() {
	vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, m_SwapChain.getVkSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	m_CommandBuffer.reset();
	m_CommandBuffer.beginRecording();

	m_RenderPass.beginRenderPass(m_CommandBuffer, swapChainFramebuffers[imageIndex], m_SwapChain.getVkSwapChainExtent());


	//float windowWidth = 800.0f;
	//float windowHeight = 600.0f;
	//float aspectRatio = windowWidth / windowHeight;

	// Near and Far planes
	//float nearPlane = 0.001f;
	//float farPlane = 100.0f;


	// 2D Camera matrix
	//ViewProjection vp2D{};
	//vp2D = //m_Camera.GetViewProjection(windowWidth, windowHeight, nearPlane, farPlane);

	// draw pipeline 1.
	//m_GraphicsPipeline2D.setUBO(vp2D, 0);
	//m_GraphicsPipeline2D.DrawFrame(m_CommandBuffer.getVkCommandBuffer(), m_SwapChain.getVkSwapChainExtent());

	m_GraphicsPipelineGrid.draw(m_CommandBuffer.getVkCommandBuffer(), m_SwapChain.getVkSwapChainExtent(), m_LineWidth);
	m_GraphicsPipelineFill.draw(m_CommandBuffer.getVkCommandBuffer(), m_SwapChain.getVkSwapChainExtent());
	m_GraphicsPipelineTemp.draw(m_CommandBuffer.getVkCommandBuffer(), m_SwapChain.getVkSwapChainExtent(), m_LineWidth);

	ViewProjection vp3D{};
	
	vp3D = m_Camera.GetProjectionViewMatrix();

	//m_GraphicsPipeline3D.setUBO(vp3D, 0);
	//m_GraphicsPipeline3D.DrawFrame(m_CommandBuffer.getVkCommandBuffer(), m_SwapChain.getVkSwapChainExtent());

	//ViewProjection vp{};
	//
	//glm::vec3 scaleFactors(1 / 400.0f, 1 / 300.0f, 1.0f);
	//vp.view = glm::scale(glm::mat4(1.0f), scaleFactors);
	//vp.view = glm::translate(vp.view, glm::vec3(-1, -1, 0));
	//
	//glm::vec3 cameraPos = glm::vec3(m_Radius * cosf(m_Rotation), -6, m_Radius * sinf(m_Rotation));
	//glm::vec3 targetPos = glm::vec3(0, 0, 0);
	//glm::vec3 upVector = glm::vec3(0, 1, 0);
	//m_Rotation += 0.001f;
	//
	//// Window dimensions
	//float windowWidth = 800.0f;
	//float windowHeight = 600.0f;
	//float aspectRatio = windowWidth / windowHeight;
	
	// Field of View
	//float fov = 45.0f; // In degrees
	//
	// Near and Far planes
	//float nearPlane = 0.1f;
	//float farPlane = 100.0f;
	
	// View matrix
	//vp.view = glm::lookAt(cameraPos, targetPos, upVector);
	
	// Projection matrix
	//vp.proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	//
	//m_GraphicsPipeline3D.setUBO(vp, 0);
	//m_GraphicsPipeline3D.DrawFrame(m_CommandBuffer.getVkCommandBuffer(), m_SwapChain.getVkSwapChainExtent());

	m_RenderPass.endRenderPass(m_CommandBuffer);
	m_CommandBuffer.endRecording();

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	elapseTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), elapseTime * glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = vp3D.view * glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = vp3D.proj; //* glm::perspective(glm::radians(45.0f), m_SwapChain.getVkSwapChainExtent().width / (float)m_SwapChain.getVkSwapChainExtent().height, 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;
	memcpy(m_Buffer.getUniformBuffersMapped()[imageIndex], &ubo, sizeof(ubo));


	//m_Buffer.updateUniformBuffer(imageIndex, m_SwapChain.getVkSwapChainExtent());

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	m_CommandBuffer.submit(submitInfo);

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_SwapChain.getVkSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(presentQueue, &presentInfo);
}

bool checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> VulkanBase::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool VulkanBase::checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void VulkanBase::createInstance() {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}