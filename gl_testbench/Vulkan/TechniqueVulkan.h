#pragma once

#include "MaterialVulkan.h"
#include "../Technique.h"
#include <vulkan\vulkan.h>

class TechniqueVulkan : public Technique
{
public:
	TechniqueVulkan(Material* m, RenderState* r);
	~TechniqueVulkan();

	void enable(Renderer* renderer);

private:
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
};