#pragma once
#include "DAESwapChain.h"
#include "vulkan/vulkan_core.h"
#include "vector"
#include "DAEBuffer.h"
class Texture;
class Depth
{
public:
	Depth() {}
	~Depth() = default;

	void createDepthResources(Texture texture, SwapChain swapChain, Buffer& buffer);
	VkFormat findDepthFormat();

	static bool hasStencilComponent(VkFormat format);

	const VkImageView& getVkDepthImageView()
	{
		return depthImageView;
	}

	void initialize(const VkPhysicalDevice& physicalDevice) { m_PhysicalDevice = physicalDevice; }
	void destroy(const VkDevice& device);
private:
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkPhysicalDevice m_PhysicalDevice;
};
