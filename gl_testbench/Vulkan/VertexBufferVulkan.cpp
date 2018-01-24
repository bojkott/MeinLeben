#include "VertexBufferVulkan.h"
#include "VulkanRenderer.h"
VertexBufferVulkan::VertexBufferVulkan(size_t size, VertexBuffer::DATA_USAGE usage)
{
	VkDeviceSize bufferSize = size;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;
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
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(VulkanRenderer::device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(VulkanRenderer::device, buffer, bufferMemory, 0);
}

VertexBufferVulkan::~VertexBufferVulkan()
{
}

void VertexBufferVulkan::setData(const void * data, size_t size, size_t offset)
{
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location)
{
}

void VertexBufferVulkan::unbind()
{
}

size_t VertexBufferVulkan::getSize()
{
	return size_t();
}
