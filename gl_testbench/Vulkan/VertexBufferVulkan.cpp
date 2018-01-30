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
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (FAILED(vkCreateBuffer(VulkanRenderer::device, &bufferInfo, nullptr, &vertexBuffer)))
	{
		fprintf(stderr, "failed to create vertex buffer!\n");
		exit(-1);
	}
	
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(VulkanRenderer::device, vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (FAILED(vkAllocateMemory(VulkanRenderer::device, &allocInfo, nullptr, &vertexBufferMemory)))
	{
		fprintf(stderr, "failed to allocate buffer memory!\n");
		exit(-1);
	}

	vkBindBufferMemory(VulkanRenderer::device, vertexBuffer, vertexBufferMemory, 0);
	
}

VertexBufferVulkan::~VertexBufferVulkan()
{
	vkDestroyBuffer(VulkanRenderer::device, vertexBuffer, nullptr);
	vkFreeMemory(VulkanRenderer::device, vertexBufferMemory, nullptr);
}

void VertexBufferVulkan::setData(const void * data, size_t size, size_t offset)
{
	void* vkData;
	auto result = vkMapMemory(VulkanRenderer::device, vertexBufferMemory, offset, size, 0, &vkData);
	memcpy(vkData, data, size);
	vkUnmapMemory(VulkanRenderer::device, vertexBufferMemory);
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { offset };
	vkCmdBindVertexBuffers(*VulkanRenderer::currentBuffer, location, 1, vertexBuffers, offsets);
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
