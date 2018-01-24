#pragma once

#include "MaterialVulkan.h"
#include "../Technique.h"
#include <vulkan\vulkan.h>

class TechniqueVulkan : public Technique
{
public:
	static TechniqueVulkan* currentTechnique;

	TechniqueVulkan(Material* m, RenderState* r);
	~TechniqueVulkan();

	void enable(Renderer* renderer);
	
private:
	VkPipeline graphicsPipeline;
	
};