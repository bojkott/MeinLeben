#include "../Sampler2D.h"
#include <vulkan\vulkan.h>
class Sampler2DVulkan : public Sampler2D
{
public:
	Sampler2DVulkan();
	~Sampler2DVulkan();

	void setMagFilter(FILTER filter);
	void setMinFilter(FILTER filter);
	void setWrap(WRAPPING s, WRAPPING t);

	VkSamplerCreateInfo samplerInfo;
private:
	
};
