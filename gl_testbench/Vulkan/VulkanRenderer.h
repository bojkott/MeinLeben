#pragma once
#include <SDL.h>
#include <vulkan\vulkan.h>
#include "../Renderer.h"


#pragma comment(lib, "vulkan-1.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")


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

	void setClearColor(float, float, float, float);
	void clearBuffer(unsigned int);
	void setRenderState(RenderState* ps);
	void submit(Mesh* mesh);
	void frame();

private:
	#ifdef _DEBUG
		const bool enableValidationLayers = true;
	#else
		const bool enableValidationLayers = false;
	#endif
	#define FAILED(x) x != VK_SUCCESS


	struct QueueFamilityIndicies {
		int graphicsFamily = -1;

		bool isComplete()
		{
			return graphicsFamily >= 0;
		}	
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_core_validation"
	};

	VkResult CreateDebugReportCallbackEXT(
		VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	void DestroyDebugReportCallbackEXT(
		VkInstance instance, VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);


	SDL_Window* window;
	VkInstance instance;
	VkDebugReportCallbackEXT callback;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;

	void initWindow(unsigned int width, unsigned int height);
	void initVulkan();
	void createInstance();
	void createLogicalDevice();
	void setupDebugCallback();
	bool checkValidationLayersSupport();
	std::vector<const char*> getRequiredExtensions();
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilityIndicies findQueueFamilies(VkPhysicalDevice device);

};
