#include "VertexBufferVulkan.h"
#include "VulkanRenderer.h"
#include "TechniqueVulkan.h"
VertexBufferVulkan::VertexBufferVulkan(size_t size, VertexBuffer::DATA_USAGE usage)
{
	bufferSize = size;
	VkDeviceSize vkBufferSize = size;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = vkBufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(VulkanRenderer::device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(VulkanRenderer::device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(VulkanRenderer::device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		fprintf(stderr, "failed to allocate buffer memory!\n");
		exit(-1);
	}

	vkBindBufferMemory(VulkanRenderer::device, buffer, bufferMemory, 0);
}

VertexBufferVulkan::~VertexBufferVulkan()
{
	vkDestroyBuffer(VulkanRenderer::device, buffer, nullptr);
	vkFreeMemory(VulkanRenderer::device, bufferMemory, nullptr);
}

void VertexBufferVulkan::setData(const void * data, size_t size, size_t offset)
{
	void* vkData;
	vkMapMemory(VulkanRenderer::device, bufferMemory, offset, size, 0, &vkData);
	memcpy(vkData, &data, size);
	vkUnmapMemory(VulkanRenderer::device, bufferMemory);
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location)
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = offset;
	bufferInfo.range = size;

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = TechniqueVulkan::currentTechnique->descriptorSet;
	descriptorWrite.dstBinding = location;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pBufferInfo = &bufferInfo;
	descriptorWrite.pImageInfo = nullptr; // Optional
	descriptorWrite.pTexelBufferView = nullptr; // Optional

	vkUpdateDescriptorSets(VulkanRenderer::device, 1, &descriptorWrite, 0, nullptr);
}

void VertexBufferVulkan::unbind()
{
}

size_t VertexBufferVulkan::getSize()
{
	return bufferSize;
}

uint32_t VertexBufferVulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(VulkanRenderer::physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	fprintf(stderr, "failed to find suitable memory type!\n");
	exit(-1);
}
