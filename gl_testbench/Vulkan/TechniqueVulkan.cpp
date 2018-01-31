#include "TechniqueVulkan.h"

#include "VulkanRenderer.h"
#include "RenderStateVulkan.h"

TechniqueVulkan* TechniqueVulkan::currentTechnique;
int TechniqueVulkan::numberOfTechniques = 0;
TechniqueVulkan::TechniqueVulkan(Material * m, RenderState * r) : Technique(m, r)
{
	id = numberOfTechniques;
	numberOfTechniques++;

	material = m;
	renderState = r;
	RenderStateVulkan* vkR = (RenderStateVulkan*)r;
	//vertex input

	std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = ((MaterialVulkan*)m)->getBindingDescriptions();
	std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = ((MaterialVulkan*)m)->getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = vertexBindingDescriptions.size();
	vertexInputInfo.pVertexBindingDescriptions = vertexBindingDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptions.size();
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();


	//input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;


	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = ((MaterialVulkan*)m)->getShaderStages();

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	


	pipelineInfo.pViewportState = vkR->getViewportState();
	pipelineInfo.pRasterizationState = vkR->getRasterizer();
	pipelineInfo.pMultisampleState = vkR->getMultisampling();
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = vkR->getColorBlending();
	pipelineInfo.pDynamicState = nullptr; // Optional

	pipelineInfo.layout = VulkanRenderer::pipelineLayout;
	pipelineInfo.renderPass = VulkanRenderer::renderPass;
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (FAILED(vkCreateGraphicsPipelines(VulkanRenderer::device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline))) {
		fprintf(stderr, "failed to create graphics pipeline!\n");
		exit(-1);
	}
}

TechniqueVulkan::~TechniqueVulkan()
{
	vkDestroyPipeline(VulkanRenderer::device, graphicsPipeline, nullptr);
}


void TechniqueVulkan::enable(Renderer * renderer)
{
	currentTechnique = this;
	vkCmdBindPipeline(*((VulkanRenderer*)renderer)->currentBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	material->enable();
}