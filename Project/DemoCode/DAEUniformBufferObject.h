#pragma once

#include "vulkanbase/VulkanUtil.h"
#include "DAEBuffer.h"

template <class UBO>
class DAEUniformBufferObject
{
public:
	void initialize(const VulkanContext& context);
	void upload();
	void setData(UBO ubo) {
		m_UBOSrc = ubo;
	}

	void destroy();

	VkBuffer getVkBuffer() {
		return m_UBOBuffer->getUniformBuffer();
	}
private:
	std::unique_ptr<Buffer> m_UBOBuffer;
	UBO m_UBOSrc;
};

template<class UBO>
inline void DAEUniformBufferObject<UBO>::initialize(const VulkanContext& context)
{
	m_UBOBuffer = std::make_unique<Buffer>(
		context,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		sizeof(UBO)
		);
}

template<class UBO>
inline void DAEUniformBufferObject<UBO>::upload()
{
	m_UBOBuffer->upload(sizeof(UBO), &m_UBOSrc);
}

template<class UBO>
inline void DAEUniformBufferObject<UBO>::destroy()
{
	m_UBOBuffer->destroy();
}

template<class UBO>
using DAEUniformBufferObjectPtr = std::unique_ptr<DAEUniformBufferObject<UBO>>;


