#pragma once

#include "../Texture2D.h"

#include <stb_image.h>
#include <vulkan\vulkan.h>
#include "VulkanRenderer.h"


class Texture2DVulkan
{
private:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

public:
	Texture2DVulkan();
	~Texture2DVulkan();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);

private:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, 
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);


	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};
