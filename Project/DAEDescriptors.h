#pragma once
#include <vector>
#include "vulkan/vulkan_core.h"
#include "DemoCode/DAEUniformBufferObject.h"
#include <array>
#include <stdexcept>
#include <Vertex.h>
template<class UBO>
class Descriptors
{
public:
	Descriptors(VkDevice device, size_t count);
	~Descriptors() = default;

	void initialize(const VulkanContext& context, const VkBuffer& uniformBuffers, VkImageView textureImageView, VkSampler textureSampler);

	void setUBO(UBO data, size_t index);

	const VkDescriptorSetLayout& getDescriptorSetLayout()
	{
		return m_DescriptorSetLayout;
	}

	void createDescriptorSets(const VkBuffer& uniformBuffers, VkImageView textureImageView, VkSampler textureSampler);

	const std::vector<VkDescriptorSet>& getDescriptorSets()
	{
		return descriptorSets;
	}

	void bindDescriptorSet(VkCommandBuffer buffer, VkPipelineLayout layout, size_t index);

	void destroy();
private:
	VkDevice m_VkDevice;
	VkDeviceSize m_Size;
	VkDescriptorSetLayout m_DescriptorSetLayout;

	void createDescriptorPool();
	void createDescriptorSetLayout(const VulkanContext& context);
	void createUBOs(const VulkanContext& context);

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<DAEUniformBufferObjectPtr<UBO>> m_UBOs;

	size_t m_Count;
};

template<class UBO>
void Descriptors<UBO>::createDescriptorSetLayout(const VulkanContext& context)
{
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();
	
	if (vkCreateDescriptorSetLayout(m_VkDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

template<class UBO>
inline Descriptors<UBO>::Descriptors(VkDevice device, size_t count)
	:
	m_VkDevice{ device },
	m_Size{ sizeof(UBO) },
	m_Count(count),
	descriptorPool{ nullptr },
	m_DescriptorSetLayout{ nullptr }
{
	createDescriptorPool();
}

template<class UBO>
void Descriptors<UBO>::initialize(const VulkanContext& context, const VkBuffer& uniformBuffers, VkImageView textureImageView, VkSampler textureSampler)
{
	createDescriptorSetLayout(context);
	createUBOs(context);
	createDescriptorSets(uniformBuffers, textureImageView, textureSampler);
}

template<class UBO>
void Descriptors<UBO>::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	
	if (vkCreateDescriptorPool(m_VkDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

template<class UBO>
void Descriptors<UBO>::createDescriptorSets(const VkBuffer& uniformBuffers, VkImageView textureImageView, VkSampler textureSampler)
{
	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);
	
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();
	
	VkDevice t = m_VkDevice;
	VkDescriptorSetAllocateInfo* x = &allocInfo;
	auto g = descriptorSets.data();
	if (vkAllocateDescriptorSets(m_VkDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
	
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);
	
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;
	
		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		//VkWriteDescriptorSet descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
	
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(m_VkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

template<class UBO>
inline void Descriptors<UBO>::setUBO(UBO data, size_t index)
{
	if (index < m_UBOs.size())
	{
		m_UBOs[index]->setData(data);
		m_UBOs[index]->upload();
	}
}

template<class UBO>
inline void Descriptors<UBO>::bindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, size_t index)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[index], 0, nullptr);
}

template<class UBO>
void Descriptors<UBO>::destroy()
{
	vkDestroyDescriptorSetLayout(m_VkDevice, m_DescriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(m_VkDevice, descriptorPool, nullptr);
	
	//for (DAEUniformBufferObjectPtr<UBO>* mesh : m_UBOs)
	//{
	//	delete mesh->destroy();
	//}

	m_UBOs[0]->destroy();

	m_UBOs.clear();
}

template<class UBO>
inline void Descriptors<UBO>::createUBOs(const VulkanContext& context)
{
	for (int uboIndex = 0; uboIndex < m_Count; ++uboIndex)
	{
		DAEUniformBufferObjectPtr<UBO> buffer = std::make_unique<DAEUniformBufferObject<UBO>>();
		buffer->initialize(context);
		m_UBOs.emplace_back(std::move(buffer));
	}
}
