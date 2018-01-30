#include "ConstantBufferVulkan.h"
#include "VulkanRenderer.h"
#include <vulkan\vulkan.h>
#include "../IA.h"
ConstantBufferVulkan::ConstantBufferVulkan(std::string NAME, unsigned int location)
{
	name = NAME;
	this->location = location;
	buff = malloc(sizeof(float)*4);
}

ConstantBufferVulkan::~ConstantBufferVulkan()
{
}

void ConstantBufferVulkan::setData(const void * data, size_t size, Material * m, unsigned int location)
{
	float x = ((float*)data)[0];
	memcpy(buff, data, size);
	this->size = size;
}

void ConstantBufferVulkan::bind(Material *)
{
	VkShaderStageFlagBits vkLocation;
	int offset = 0;
	switch (location)
	{
	case TRANSLATION:
		offset = 0;
		vkLocation = VK_SHADER_STAGE_VERTEX_BIT;
		break;
	case DIFFUSE_TINT:
		offset = sizeof(float) * 4;
		vkLocation = VK_SHADER_STAGE_FRAGMENT_BIT;
		break;
	default:
		break;
	}
	vkCmdPushConstants(*VulkanRenderer::currentBuffer, VulkanRenderer::pipelineLayout, vkLocation, offset, size, buff);
}
