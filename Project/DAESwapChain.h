#pragma once

#include <optional>

#include "DAEDevice.h"
#include "vulkan/vulkan_core.h"
#include "vector"
#include "GLFW/glfw3.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain
{
public:
	SwapChain() {}
	~SwapChain() = default;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, const VkSurfaceKHR& surface);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	void createSwapChain(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, GLFWwindow* window, Device deviceClass);
	void createImageViews(const VkDevice& device);

	VkExtent2D getVkSwapChainExtent() const {
		return swapChainExtent;
	}
	VkFormat getVkSwapChainImageFormat() const {
		return swapChainImageFormat;
	}
	VkSwapchainKHR getVkSwapChain() const {
		return swapChain;
	}
	std::vector<VkImageView> getVkSwapChainImageViews() const {
		return swapChainImageViews;
	}

	void initialize();
	void destroy();
private:

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;
};