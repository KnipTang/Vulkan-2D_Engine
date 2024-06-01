#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "VulkanUtil.h"
#include "DAEShader3D.h"
#include "DAEShader2D.h"
#include "DAECommandPool.h"
#include "DAECommandBuffer.h"
#include "DAERenderPass.h"
#include "DAEBuffer.h"
#include "DAEMesh3D.h"
#include "Scene.h"
#include "DAEDescriptors.h"
#include "DAEDepth.h"
#include "DAETexture.h"
#include "DAEDevice.h"
#include <glm/glm.hpp>
#include "DAECamera.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <array>
#include "Vertex.h"
#include "DAEGraphicsPipeline.h"
#include "DAEGraphicsPipeline3D.h"
#include "DAEMesh2D.h"
#include "DAEShader2DFill.h"
#include "2D/DAEGraphicsPipelineTemp.h"
#include "DAERenderPass2D.h"
#include <2D/DAEGraphicsPipelineFill.h>
#include "SceneFill.h"
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanBase {
public:
	void run() {
		//for visualising the rendering
		initWindow();
		//for initialising objects
		initVulkan();
		//main rendering loop
		mainLoop();
		//sort of a destructor for the objects created in initVulkan
		cleanup();
	}

private:
	void initVulkan() {
		m_Camera.Initialize(45.f, { 0,0,0 }, (float)WIDTH / (float)HEIGHT);

		// week 06
		createInstance();
		setupDebugMessenger();
		createSurface();

		// week 05
		pickPhysicalDevice();
		createLogicalDevice();

		// week 04 
		m_SwapChain.createSwapChain(device, physicalDevice, surface, window, m_Device);
		m_SwapChain.createImageViews(device);
		
		//m_RenderPass2D.initialize(device);
		//m_RenderPass2D.createRenderPass(m_SwapChain.getVkSwapChainImageFormat());
		
		// week 03
		m_Depth.initialize(physicalDevice);
		m_RenderPass.initialize(device);
		m_RenderPass.createRenderPass(m_SwapChain.getVkSwapChainImageFormat(), m_Depth);
		//m_Shader3D.initialize(device);
		m_Shader2DFill.initialize(device);
		m_Shader2D.initialize(device);
		m_Shader2DGrid.initialize(device);
		// week 02
		m_CommandPool.initialize(device, m_Device.findQueueFamilies(physicalDevice, surface));
		m_Buffer.initialize(device, physicalDevice, m_CommandPool.getVkCommandPool(), graphicsQueue);
		m_Texture.initialize(device);
		m_Depth.createDepthResources(m_Texture, m_SwapChain, m_Buffer);
		createFrameBuffers();
		m_Texture.createTextureImage(m_Buffer);
		m_Texture.createTextureImageView();
		m_Texture.createTextureSampler(physicalDevice);
		//m_Mesh3D.loadModel();
		//m_Mesh3D.createVertexBuffer(graphicsQueue,m_Buffer, m_CommandPool.getVkCommandPool());
		//m_Mesh3D.createIndexBuffer(graphicsQueue, m_Buffer, m_CommandPool.getVkCommandPool());
		//m_Mesh3D2.loadModel();
		//m_Mesh3D2.createVertexBuffer(graphicsQueue, m_Buffer, m_CommandPool.getVkCommandPool());
		//m_Mesh3D2.createIndexBuffer(graphicsQueue, m_Buffer, m_CommandPool.getVkCommandPool());

		m_Mesh2DFill.createVertexBuffer(m_TorusFillVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DFill.createIndexBuffer(m_TorusFillVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D2Fill.createVertexBuffer(m_RoundedRectFillVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D2Fill.createIndexBuffer(m_RoundedRectFillVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D3Fill.createVertexBuffer(m_OvalFillVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D3Fill.createIndexBuffer(m_OvalFillVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D4Fill.createVertexBuffer(m_RectFillVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D4Fill.createIndexBuffer(m_RectFillVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D5Fill.createVertexBuffer(m_ArcFillVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D5Fill.createIndexBuffer(m_ArcFillVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());

		m_Mesh2D.createVertexBuffer(m_TorusVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D.createIndexBuffer(m_TorusVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D2.createVertexBuffer(m_RoundedRectVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D2.createIndexBuffer(m_RoundedRectVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D3.createVertexBuffer(m_OvalVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D3.createIndexBuffer(m_OvalVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D4.createVertexBuffer(m_RectVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D4.createIndexBuffer(m_RectVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D5.createVertexBuffer(m_ArcVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2D5.createIndexBuffer(m_ArcVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineVer.createVertexBuffer(m_LineVerticalVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineVer.createIndexBuffer(m_LineVerticalVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineHor.createVertexBuffer(m_LineHorizontalVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineHor.createIndexBuffer(m_LineHorizontalVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineVerLeft.createVertexBuffer(m_LineVerticalLeftVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineVerLeft.createIndexBuffer(m_LineVerticalLeftVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineHorTop.createVertexBuffer(m_LineHorizontalTopVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineHorTop.createIndexBuffer(m_LineHorizontalTopVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineVerRight.createVertexBuffer(m_LineVerticalRightVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineVerRight.createIndexBuffer(m_LineVerticalRightVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineHorBottom.createVertexBuffer(m_LineHorizontalBottomVerInd.vertices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());
		m_Mesh2DLineHorBottom.createIndexBuffer(m_LineHorizontalBottomVerInd.indices, m_Buffer, graphicsQueue, m_CommandPool.getVkCommandPool());

		m_Buffer.createUniformBuffers();
		//VulkanContext& m_Context = VulkanContext{ device, physicalDevice, m_RenderPass.getVkRenderPass(), m_SwapChain.getVkSwapChainExtent() };
		//m_GraphicsPipeline3D.initialize(m_Context, m_Buffer, m_Texture );
		//m_GraphicsPipeline3D.createGraphicsPipeline(m_RenderPass.getVkRenderPass(), m_Shader3D);
		//m_GraphicsPipeline3D.addMesh(m_Mesh3D);
		//m_GraphicsPipeline3D.addMesh(m_Mesh3D2);
		
		m_GraphicsPipelineGrid.initialize(device);
		m_GraphicsPipelineGrid.createGraphicsPipeline(m_RenderPass.getVkRenderPass(), m_Shader2DGrid, &m_LineWidthGrid);
		m_GraphicsPipelineGrid.addMesh(m_Mesh2DLineVer);
		m_GraphicsPipelineGrid.addMesh(m_Mesh2DLineHor);
		m_GraphicsPipelineGrid.addMesh(m_Mesh2DLineVerLeft);
		m_GraphicsPipelineGrid.addMesh(m_Mesh2DLineHorTop);
		m_GraphicsPipelineGrid.addMesh(m_Mesh2DLineVerRight);
		m_GraphicsPipelineGrid.addMesh(m_Mesh2DLineHorBottom);

		m_GraphicsPipelineFill.initialize(device);
		m_GraphicsPipelineFill.createGraphicsPipeline(m_RenderPass.getVkRenderPass(), m_Shader2DFill);
		//m_GraphicsPipelineFill.addMesh(m_Mesh2DFill);
		m_GraphicsPipelineFill.addMesh(m_Mesh2D2Fill);
		//m_GraphicsPipelineFill.addMesh(m_Mesh2D3Fill);
		//m_GraphicsPipelineFill.addMesh(m_Mesh2D4Fill);
		//m_GraphicsPipelineFill.addMesh(m_Mesh2D5Fill);
		m_GraphicsPipelineTemp.initialize(device);
		m_GraphicsPipelineTemp.createGraphicsPipeline(m_RenderPass.getVkRenderPass(), m_Shader2D, &m_LineWidth);
		//m_GraphicsPipelineTemp.addMesh(m_Mesh2D);
		m_GraphicsPipelineTemp.addMesh(m_Mesh2D2);


		//m_GraphicsPipelineTemp.addMesh(m_Mesh2D3);
		//m_GraphicsPipelineTemp.addMesh(m_Mesh2D4);
		//m_GraphicsPipelineTemp.addMesh(m_Mesh2D5);


		m_CommandBuffer = m_CommandPool.createCommandBuffer();

		msaaSamples = getMaxUsableSampleCount();

		// week 06
		createSyncObjects();
	}

	void mainLoop() {
		//Keeps looping until X(Exit) button of window gets pressed
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			// week 06
			drawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	void cleanup() {
		//cleanupSwapChain();

		//m_Mesh3D.destory();
		//m_Mesh3D2.destory();
		m_Mesh2D.destory();
		m_Mesh2D2.destory();
		m_Mesh2D3.destory();
		m_Mesh2D4.destory();
		m_Mesh2D5.destory();
		m_Mesh2DLineVer.destory();
		m_Mesh2DLineHor.destory();
		m_Mesh2DLineVerLeft.destory();
		m_Mesh2DLineHorTop.destory();
		m_Mesh2DLineVerRight.destory();
		m_Mesh2DLineHorBottom.destory();

		m_Buffer.destroy();

		vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
		vkDestroyFence(device, inFlightFence, nullptr);

		//vkDestroyCommandPool(device, m_CommandPool.getVkCommandPool(), nullptr);
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}

		//m_GraphicsPipeline3D.destroy();
		m_GraphicsPipelineTemp.destroy();
		m_GraphicsPipelineFill.destroy();
		m_GraphicsPipelineGrid.destroy();

		m_RenderPass.destroy();
		//m_RenderPass2D.destroy();

		m_CommandPool.destroy();
		

		for (auto imageView : m_SwapChain.getVkSwapChainImageViews()) {
			vkDestroyImageView(device, imageView, nullptr);
		}

		m_Depth.destroy(device);

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

		vkDestroySwapchainKHR(device, m_SwapChain.getVkSwapChain(), nullptr);

		m_Texture.destroy();

		vkDestroyDevice(device, nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		//Clean up of window
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void createSurface() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	// Week 01: 
	// Actual window
	// simple fragment + vertex shader creation functions
	// These 5 functions should be refactored into a separate C++ class
	// with the correct internal state.

	//DAEShader3D m_Shader3D{ 
	//	"shaders/shader.vert.spv", 
	//	"shaders/shader.frag.spv" 
	//};

	DAEShader2D m_Shader2DGrid{
	"shaders/shader2D.vert.spv",
	"shaders/shader2D.frag.spv"
	};

	DAEShader2D m_Shader2D{
		"shaders/shader2D.vert.spv",
		"shaders/shader2D.frag.spv"
	};

	DAEShader2DFill m_Shader2DFill{
		"shaders/shader2D.vert.spv",
		"shaders/shader2D.frag.spv"
	};

	GLFWwindow* window;
	void initWindow();

	// Week 02
	// Queue families
	// CommandBuffer concept
	DAECommandPool m_CommandPool{};
	DAECommandBuffer m_CommandBuffer{};

	void drawFrame(uint32_t imageIndex);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	// Week 03
	// Renderpass concept
	// Graphics pipeline
	Buffer m_Buffer{};

	Scene m_Scene{};
	SceneFill m_SceneFill{};

	std::vector<VkFramebuffer> swapChainFramebuffers;

	//GraphicsPipeline3D m_GraphicsPipeline3D{};
	RenderPass m_RenderPass{};
	//RenderPass2D m_RenderPass2D{};
	GraphicsPipelineTemp m_GraphicsPipelineTemp{};
	GraphicsPipelineFill m_GraphicsPipelineFill{};
	GraphicsPipelineTemp m_GraphicsPipelineGrid{};

	void createFrameBuffers();

	const VerInd m_LineVerticalVerInd = Scene::generateLine(0.f, 1.f, 0.0f, -1.f, { 0,0,.5f });
	const VerInd m_LineHorizontalVerInd = Scene::generateLine(1.f, 0.f, -1.0f, 0.f, { 0,0,.5f });
	const VerInd m_LineVerticalLeftVerInd = Scene::generateLine(-0.5f, 1.f, -0.5f, -1.f, { 0,0,.5f });
	const VerInd m_LineHorizontalTopVerInd = Scene::generateLine(1.f, -0.5f, -1.0f, -0.5f, { 0,0,.5f });
	const VerInd m_LineVerticalRightVerInd = Scene::generateLine(0.5f, 1.f, 0.5f, -1.f, { 0,0,.5f });
	const VerInd m_LineHorizontalBottomVerInd = Scene::generateLine(1.f, 0.5f, -1.0f, 0.5f, { 0,0,.5f });

	const VerInd m_RectVerInd = Scene::generateRectangle(-0.9f, -0.9f, 0.5f,0.5f);
	const VerInd m_OvalVerInd = Scene::generateOval(0.70f, 0.70f, 0.25f, 16);
	const VerInd m_ArcVerInd = Scene::generateArc(0.70f, -0.70f, 0.25f, 16, 300.f);
	const VerInd m_TorusVerInd = Scene::generateDonut(0.70f, 0.f, 0.25f, 0.15f, 16);
	const VerInd m_RoundedRectVerInd = Scene::generateRoundedRectangle(-.5f, -.4f, .1f, .3f, 50);

	const VerInd m_RectFillVerInd = SceneFill::generateRectangle(-0.9f, -0.9f, 0.5f, 0.5f, { 1,0,0 });
	const VerInd m_OvalFillVerInd = SceneFill::generateOval(0.70f, 0.70f, 0.25f, 16, { 1,0,0 });
	const VerInd m_ArcFillVerInd = SceneFill::generateArc(0.70f, -0.70f, 0.25f, 16, 300.f, { 1,0,0 });
	const VerInd m_TorusFillVerInd = SceneFill::generateDonut(0.70f, 0.f, 0.25f, 0.15f, 16, { 1,0,0 });
	const VerInd m_RoundedRectFillVerInd = SceneFill::generateRoundedRectangle(-.5f, -.4f, .1f, .3f, 50, { 1,0,0 });

	// Week 04
	// Swap chain and image view support
	//Mesh3D m_Mesh3D{ device,  graphicsQueue, m_CommandPool.getVkCommandPool(), "models/viking_room.obj"};
	//Mesh3D m_Mesh3D2{ device,  graphicsQueue, m_CommandPool.getVkCommandPool(), "models/viking_room4.obj"};
	//2D
	Mesh2D m_Mesh2DLineVer{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2DLineHor{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2DLineVerLeft{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2DLineHorTop{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2DLineVerRight{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2DLineHorBottom{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };

	Mesh2D m_Mesh2D{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D2{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D3{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D4{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D5{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };

	Mesh2D m_Mesh2DFill{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D2Fill{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D3Fill{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D4Fill{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };
	Mesh2D m_Mesh2D5Fill{ device, graphicsQueue, m_CommandPool.getVkCommandPool() };

	SwapChain m_SwapChain{};

	Depth m_Depth{};

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();

	// Week 06
	// Main initialization
	Texture m_Texture{};

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDevice device = VK_NULL_HANDLE;

	Device m_Device{};

	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	VkSampleCountFlagBits msaaSamples;
	VkSampleCountFlagBits getMaxUsableSampleCount() {
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}
	//void createColorResources() {
	//	VkFormat colorFormat = m_SwapChain.getVkSwapChainImageFormat();
	//
	//	m_Texture.createImage(m_SwapChain.getVkSwapChainExtent().width, m_SwapChain.getVkSwapChainExtent().height, msaaSamples, 1, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	//	colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	//}
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	//Camera
	Camera m_Camera;
	float m_CameraRadius;
	glm::vec3 m_DragStart;
	float m_Rotation;
	double m_Lag = 0;
	float m_Radius = 1;

	float elapseTime;

	float m_LineWidth = 5.f;
	glm::vec3 m_ColorLine = { 0,1,0 };

	glm::vec3 m_ColorFill = { 1,0,0 };

	bool m_ToggleGrid = true;
	float m_LineWidthGrid = 1.f;
	glm::vec3 m_ColorGrid = { 0,0,1 };


	void keyEvent(int key, int scancode, int action, int mods);
	void mouseMove(GLFWwindow* window, double xpos, double ypos);
	void mouseEvent(GLFWwindow* window, int button, int action, int mods);

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createInstance();

	void createSyncObjects();
	void drawFrame();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};