#pragma once
#include "vulkan/vulkan_core.h"

class DAECommandBuffer {
public:
	DAECommandBuffer() {}
	~DAECommandBuffer() = default;

	void setVkCommandBuffer(VkCommandBuffer buffer) {
		m_CommandBuffer = buffer;
	}

	VkCommandBuffer getVkCommandBuffer()const {
		return m_CommandBuffer;
	}

	void reset()const;
	void beginRecording()const;
	void endRecording()const;
	
	void submit(VkSubmitInfo & info)const;
private:
	VkCommandBuffer m_CommandBuffer;
	
};