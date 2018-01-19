#pragma once
#include "../Material.h"
#include <vulkan\vulkan.h>
class MaterialVulkan : public Material
{
public:
	MaterialVulkan(const std::string& name);
	~MaterialVulkan();

	void setShader(const std::string& shaderFileName, ShaderType type);
	void removeShader(ShaderType type);

	void setDiffuse(Color c);

	int compileMaterial(std::string& errString);

	void addConstantBuffer(std::string name, unsigned int location);
	void updateConstantBuffer(const void* data, size_t size, unsigned int location);

	int enable();
	void disable();

private:
	int compileShader(ShaderType type, std::string& errString);
	
	VkShaderModule shaderObjects[4] = { NULL, NULL, NULL, NULL };
	VkPipelineShaderStageCreateInfo shaderStages[4];

	std::string expandShaderText(std::string& shaderText, ShaderType type);
};
