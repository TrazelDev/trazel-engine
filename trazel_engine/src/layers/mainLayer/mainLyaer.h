#pragma once
#include "../layer.h"
#include "vulkan_setup/device/swapchain.h"


class mainLyaer : public layer
{
public:
	mainLyaer(GLFWwindow* Window, const char* Title, uint32_t Width, uint32_t Height)
	{
		window = Window;
		title = Title;
		width = width;
		height = Height;
	}
	~mainLyaer();

	virtual void onAttach() override;
	virtual void onDetach() override {};
	virtual void onUpdate() override;
	virtual void onEvent() override {};

private:
	// frames pramters:
	double lastTime, currentTime;
	int numFrames;
	float frameTime;

	// vulkan instance: 
	vk::Instance instance{ nullptr };
	vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
	vk::DispatchLoaderDynamic dldi;

	// device related variables:
	vk::PhysicalDevice physicalDevice{ nullptr };
	vk::Device device{ nullptr };
	vk::Queue graphicsQueue{ nullptr };
	vk::SurfaceKHR surface;
	vk::Queue presentQueue;
	vk::SwapchainKHR swapchain;
	std::vector<vkUtil::SwapchainFrame> swapchainFrames;
	vk::Format swapchainFormat;
	vk::Extent2D swapchainExtent;

	// pipeline related variables:
	vk::PipelineLayout layout;
	vk::RenderPass renderpass;
	vk::Pipeline pipeline;

	// command related variables:
	vk::CommandPool commandPool;
	vk::CommandBuffer commandBuffer;

	// descriptorPool related variables:
	VkDescriptorPool descriptorPool;

	//syncchoronzation related variables:
	int maxFramesInFlight, frameNum;

	// instance setup
	void makeInstance();

	// device setup
	void makeDevice();

	// pipeline setup
	void makePipeline();

	void finalSetup();

	void recordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);

	void render();
	void calculateFrameRate();

};

