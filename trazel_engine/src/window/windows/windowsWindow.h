#pragma once
#include "pch.h"
#include "vulkan_setup/instance/instance.h"
#include "vulkan_setup/device/device.h"
#include "vulkan_setup/device/swapchain.h"
#include "vulkan_setup/pipeline/pipeline.h"
#include "vulkan_setup/frameBuffer/frameBuffer.h"
#include "vulkan_setup/commands/commands.h"
#include "vulkan_setup/synchronization/sync.h"

class windowsWindow
{
public:
	windowsWindow(int Width = 1280, int Height = 720, const char* Title = "TRAZEL_ENGINE");
	~windowsWindow();
	void run();

private:
	// the glfw window parameters:
	uint32_t width;
	uint32_t height;
	const char* title;
	GLFWwindow* window;

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


	//syncchoronzation related variables:
	vk::Fence inFlightFence;
	vk::Semaphore imageAvailable, renderFinished;

	// creating glfw window
	void windowsWindow::buildGLFWWindow(int width, int height);

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
