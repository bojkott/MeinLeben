#pragma once
#pragma comment(lib, "vulkan-1.lib")
#include <vulkan\vulkan.h>
#include "../Renderer.h"

class VulkanRenderer :
	public Renderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	Material* makeMaterial(const std::string& name);
	Mesh* makeMesh();
	VertexBuffer* makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage) ;
	Texture2D* makeTexture2D();
	Sampler2D* makeSampler2D();
	RenderState* makeRenderState();
	std::string getShaderPath();
	std::string getShaderExtension();
	ConstantBuffer* makeConstantBuffer(std::string NAME, unsigned int location);
	Technique* makeTechnique(Material*, RenderState*);

	int initialize(unsigned int width = 800, unsigned int height = 600);
	void setWinTitle(const char* title);
	void present();
	int shutdown();

	virtual void setClearColor(float, float, float, float);
	virtual void clearBuffer(unsigned int);
	// can be partially overriden by a specific Technique.
	virtual void setRenderState(RenderState* ps);
	// submit work (to render) to the renderer.
	virtual void submit(Mesh* mesh);
	virtual void frame();
};
