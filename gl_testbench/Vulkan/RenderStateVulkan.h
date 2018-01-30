#pragma once
#include "../RenderState.h"
#include <vulkan\vulkan.h>

class RenderStateVulkan : public RenderState
{
public:
	RenderStateVulkan();

	void setWireFrame(bool);

	void set();

	VkPipelineViewportStateCreateInfo* getViewportState();
	VkPipelineRasterizationStateCreateInfo* getRasterizer();
	VkPipelineMultisampleStateCreateInfo* getMultisampling();
	VkPipelineColorBlendStateCreateInfo* getColorBlending();

private:
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewportState;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlending;
};