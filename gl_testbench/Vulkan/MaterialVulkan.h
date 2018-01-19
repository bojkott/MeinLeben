
#include "../Material.h"
class MaterialVulkan :
	public Material
{
public:
	MaterialVulkan();
	~MaterialVulkan();

	void setShader(const std::string& shaderFileName, ShaderType type);
	void removeShader(ShaderType type);

	void setDiffuse(Color c);

	int compileMaterial(std::string& errString);

	void addConstantBuffer(std::string name, unsigned int location) = 0;
	void updateConstantBuffer(const void* data, size_t size, unsigned int location);

	int enable();
	void disable();
};
