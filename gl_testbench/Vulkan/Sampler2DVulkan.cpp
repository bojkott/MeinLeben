#include "Sampler2DVulkan.h"

VkSamplerAddressMode wrapMap[2] = { VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE };
VkFilter filterMap[2] = { VK_FILTER_NEAREST, VK_FILTER_LINEAR };

Sampler2DVulkan::Sampler2DVulkan()
{
	samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 1.0;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;
}

Sampler2DVulkan::~Sampler2DVulkan()
{
}

void Sampler2DVulkan::setMagFilter(FILTER filter)
{
	samplerInfo.magFilter = filterMap[filter];
}

void Sampler2DVulkan::setMinFilter(FILTER filter)
{
	samplerInfo.minFilter = filterMap[filter];
}

void Sampler2DVulkan::setWrap(WRAPPING s, WRAPPING t)
{
	samplerInfo.addressModeU = wrapMap[s];
	samplerInfo.addressModeV = wrapMap[t];
}
