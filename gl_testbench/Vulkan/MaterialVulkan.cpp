#include "MaterialVulkan.h"
#include <windows.h>
#include <streambuf>
#include <sstream>
#include <istream>
#include <fstream>
#include <vector>
#include <set>
#include <assert.h>
#include <shaderc\shaderc.hpp>
#include <iostream>
#include "VulkanRenderer.h"
#include "../IA.h"

MaterialVulkan::MaterialVulkan(const std::string& name)
{
	isValid = false;

}


MaterialVulkan::~MaterialVulkan()
{
	removeShader(ShaderType::VS);
	removeShader(ShaderType::PS);
}

void MaterialVulkan::setShader(const std::string & shaderFileName, ShaderType type)
{
	if (shaderFileNames.find(type) != shaderFileNames.end())
	{
		removeShader(type);
	}
	shaderFileNames[type] = shaderFileName;
}

void MaterialVulkan::removeShader(ShaderType type)
{
	if (shaderObjects[(int)type] != NULL)
	{
		vkDestroyShaderModule(VulkanRenderer::device, shaderObjects[(int)type], nullptr);
		shaderObjects[(int)type] = NULL;
		shaderStages[(int)type] = {};
	}
	
}

void MaterialVulkan::setDiffuse(Color c)
{
}

int MaterialVulkan::compileMaterial(std::string & errString)
{
	//Remove existing shaders
	removeShader(ShaderType::VS);
	removeShader(ShaderType::PS);

	// compile shaders
	std::string err;
	if (compileShader(ShaderType::VS, err) < 0) {
		errString = err;
		fprintf(stderr, "%s", errString.c_str());
		exit(-1);
	};
	if (compileShader(ShaderType::PS, err) < 0) {
		errString = err;
		fprintf(stderr, "%s", errString.c_str());
		exit(-1);
	};
	
	//link the shaders
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = shaderObjects[(int)ShaderType::VS];
	vertShaderStageInfo.pName = "main";
	shaderStages[(int)ShaderType::VS] = vertShaderStageInfo;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = shaderObjects[(int)ShaderType::PS];;
	fragShaderStageInfo.pName = "main";
	shaderStages[(int)ShaderType::PS] = fragShaderStageInfo;
	
	return 0;
}

void MaterialVulkan::addConstantBuffer(std::string name, unsigned int location)
{
	constantBuffers[location] = new ConstantBufferVulkan(name, location);
}

void MaterialVulkan::updateConstantBuffer(const void * data, size_t size, unsigned int location)
{
	constantBuffers[location]->setData(data, size, this, location);
}

int MaterialVulkan::enable()
{
	for (auto cb : constantBuffers)
	{
		cb.second->bind(this);
	}
	return 0;
}

void MaterialVulkan::disable()
{
}
VkPipelineShaderStageCreateInfo * MaterialVulkan::getShaderStages()
{
	return shaderStages;
}

int MaterialVulkan::compileShader(ShaderType type, std::string & errString)
{
	// open the file and read it to a string "shaderText"
	std::ifstream shaderFile(shaderFileNames[type]);
	std::string shaderText;
	if (shaderFile.is_open()) {
		shaderText = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
	}
	else {
		errString = "Cannot find file: " + shaderFileNames[type];
		return -1;
	}

	// make final string with shader source + defines + GLSL version + pragma shader type.
	// in theory this uses move semantics (compiler does it automagically)
	std::string expandedShader = expandShaderText(shaderText, type);


	shaderc_shader_kind shaderType;
	switch (type)
	{
	case ShaderType::VS:
		shaderType = shaderc_glsl_vertex_shader;
		break;
	case ShaderType::PS:
		shaderType = shaderc_glsl_fragment_shader;
		break;
	case ShaderType::GS:
		shaderType = shaderc_glsl_geometry_shader;
		break;
	case ShaderType::CS:
		shaderType = shaderc_glsl_compute_shader;
		break;
	default:
		shaderType = shaderc_glsl_infer_from_source;
		break;
	}

	shaderc::Compiler spirVCompiler;
	shaderc::CompileOptions options;
	shaderc::SpvCompilationResult result = spirVCompiler.CompileGlslToSpv(expandedShader, shaderType, shaderFileNames[type].c_str());
	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		errString = "Cannot compile shader " + shaderFileNames[type] + "\n error: " + result.GetErrorMessage();
		return -1;
	}

	std::vector<uint32_t> binaryShader(result.cbegin(), result.cend());
	//binaryShader.resize(binaryShader.size() + (4 - binaryShader.size() % 4)); //pad spir-v binary to be multiple of 4.
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = binaryShader.size() * sizeof(uint32_t);
	createInfo.pCode = binaryShader.data();
	
	VkShaderModule shaderModule;
	if (FAILED(vkCreateShaderModule(VulkanRenderer::device, &createInfo, nullptr, &shaderModule)))
	{
		errString = "Failed to create shader module for shader: " + shaderFileNames[type];
		return -1;
	}
	
	shaderObjects[(int)type] = shaderModule;
	
	return 0;
}

std::string MaterialVulkan::expandShaderText(std::string & shaderText, ShaderType type)
{
	std::string result = "\n\n #version 450\n\0";
	for (auto define : shaderDefines[type])
	{
		result += define + "\n";
	}
	result += shaderText;

	return result;
}

std::vector<VkVertexInputBindingDescription> MaterialVulkan::getBindingDescriptions()
{

	std::vector<VkVertexInputBindingDescription> bindingDescriptions;

	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = POSITION;
	bindingDescription.stride = sizeof(float)*4;
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	bindingDescriptions.push_back(bindingDescription);

	bindingDescription.binding = TEXTCOORD;
	bindingDescription.stride = sizeof(float) * 2;
	bindingDescriptions.push_back(bindingDescription);

	bindingDescription.binding = NORMAL;
	bindingDescription.stride = sizeof(float) * 4;
	bindingDescriptions.push_back(bindingDescription);

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> MaterialVulkan::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

	VkVertexInputAttributeDescription attributeDescription = {};
	attributeDescription.binding = POSITION;
	attributeDescription.location = POSITION;
	attributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescription.offset = 0;
	attributeDescriptions.push_back(attributeDescription);

	attributeDescription.binding = TEXTCOORD;
	attributeDescription.location = TEXTCOORD;
	attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescription.offset = 0;
	attributeDescriptions.push_back(attributeDescription);

	attributeDescription.binding = NORMAL;
	attributeDescription.location = NORMAL;
	attributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescription.offset = 0;
	attributeDescriptions.push_back(attributeDescription);

	return attributeDescriptions;
}
