#include "VulkanRenderer.h"
#include <SDL_vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VulkanRenderer::VulkanRenderer()
{
}


VulkanRenderer::~VulkanRenderer()
{
}

Material * VulkanRenderer::makeMaterial(const std::string & name)
{
	return nullptr;
}

Mesh * VulkanRenderer::makeMesh()
{
	return nullptr;
}

VertexBuffer * VulkanRenderer::makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage)
{
	return nullptr;
}

Texture2D * VulkanRenderer::makeTexture2D()
{
	return nullptr;
}

Sampler2D * VulkanRenderer::makeSampler2D()
{
	return nullptr;
}

RenderState * VulkanRenderer::makeRenderState()
{
	return nullptr;
}

std::string VulkanRenderer::getShaderPath()
{
	return std::string();
}

std::string VulkanRenderer::getShaderExtension()
{
	return std::string();
}

ConstantBuffer * VulkanRenderer::makeConstantBuffer(std::string NAME, unsigned int location)
{
	return nullptr;
}

Technique * VulkanRenderer::makeTechnique(Material *, RenderState *)
{
	return nullptr;
}

int VulkanRenderer::initialize(unsigned int width, unsigned int height)
{
	initWindow(width, height);
	initVulkan();
	return 0;
}

void VulkanRenderer::setWinTitle(const char * title)
{
	SDL_SetWindowTitle(this->window, title);
}

void VulkanRenderer::present()
{
}

int VulkanRenderer::shutdown()
{
	vkDestroyInstance(instance, nullptr);
	SDL_Quit();
	return 0;
}

void VulkanRenderer::setClearColor(float, float, float, float)
{
}

void VulkanRenderer::clearBuffer(unsigned int)
{
}

void VulkanRenderer::setRenderState(RenderState * ps)
{
}

void VulkanRenderer::submit(Mesh * mesh)
{
}

void VulkanRenderer::frame()
{
}

void VulkanRenderer::initWindow(unsigned int width, unsigned int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "%s", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow("Vulkan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);
}

void VulkanRenderer::initVulkan()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Testbench";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;


	unsigned int extensionCount;
	SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, NULL);
	const char ** extensionNames = (const char **)malloc(sizeof(const char*) * extensionCount);
	SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames);

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensionNames;
	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		free(extensionNames);
		fprintf(stderr, "Failed to create Vulkan instance");
		exit(-1);
	}
	free(extensionNames);

	
	//const char* instanceExtensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME };
}
