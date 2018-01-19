#include "MaterialVulkan.h"



MaterialVulkan::MaterialVulkan()
{
}


MaterialVulkan::~MaterialVulkan()
{
}

void MaterialVulkan::setShader(const std::string & shaderFileName, ShaderType type)
{
}

void MaterialVulkan::removeShader(ShaderType type)
{
}

void MaterialVulkan::setDiffuse(Color c)
{
}

int MaterialVulkan::compileMaterial(std::string & errString)
{
	return 0;
}

void MaterialVulkan::addConstantBuffer(std::string name, unsigned int location)
{
}

void MaterialVulkan::updateConstantBuffer(const void * data, size_t size, unsigned int location)
{
}

int MaterialVulkan::enable()
{
	return 0;
}

void MaterialVulkan::disable()
{
}