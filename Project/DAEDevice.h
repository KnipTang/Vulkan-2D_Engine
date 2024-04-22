#pragma once

#include <optional>

#include "vulkan/vulkan_core.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class Device
{
public:
	Device() {}
	~Device() = default;

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface);

	void initialize();
	void destroy();
private:

};