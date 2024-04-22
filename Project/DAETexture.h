#pragma once
#include <string>

#include "vulkan/vulkan_core.h"
#include "DAEBuffer.h"

class Texture
{
public:
	Texture() {}
	~Texture() = default;

	void createTextureImage(Buffer& buffer);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, Buffer& buffer);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, Buffer& buffer);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, Buffer& bufferClass);

	void createTextureImageView();

	void createTextureSampler(VkPhysicalDevice& physicalDevice);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	const VkImageView& getTextureImageView()
	{
		return textureImageView;
	}
	const VkSampler& getTextureSampler()
	{
		return textureSampler;
	}

	void initialize(const VkDevice& device) { m_VkDevice = device; }
	void destroy();
private:
	VkDevice m_VkDevice;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;
	VkSampler textureSampler;

	//const uint32_t WIDTH = 800;
	//const uint32_t HEIGHT = 600;

	const std::string TEXTURE_PATH = "textures/viking_room.png";
	//const std::string TEXTURE_PATH = "textures/texture.jpg";
};
