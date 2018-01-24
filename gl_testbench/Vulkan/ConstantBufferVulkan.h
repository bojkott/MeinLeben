#pragma once
#include "../ConstantBuffer.h"

class ConstantBufferVulkan : public ConstantBuffer
{
public:
	ConstantBufferVulkan(std::string NAME, unsigned int location);
	~ConstantBufferVulkan();
	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material*);
};