#include "ConstantBufferVulkan.h"
#include "VulkanRenderer.h"
#include <vulkan\vulkan.h>
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
	vkCmdPushConstants(*VulkanRenderer::currentBuffer, VulkanRenderer::pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, size, buff);
}
