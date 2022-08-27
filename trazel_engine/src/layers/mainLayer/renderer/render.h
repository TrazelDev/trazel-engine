#pragma once
#include "../vulkan_setup/device/swapchain.h"

namespace tze
{
	class renderer
	{
	public:
		renderer(GLFWwindow& Window, vk::PhysicalDevice& PhysicalDevice, vk::Device& Device) : glfwWindow(Window), physicalDevice(PhysicalDevice), device(Device)
		{

		}
		~renderer();

		bool isFrameInProgress() const { return isFrameStarted; }
		vk::CommandBuffer getCurrentCommandBuffer() const 
		{ 
			assert(isFrameStarted && "cannot get command buffer when frame no in progress");
			return commandBuffers[currenImageIndex]; 
		}

		vk::CommandBuffer beginFrame();
		void endFrame();

		void beginSwapChain(vk::CommandBuffer commandBuffer);
		void endSwapChain(vk::CommandBuffer commandBuffer);

	private:
		GLFWwindow& glfwWindow;

		// device related variables:
		vk::PhysicalDevice& physicalDevice;
		vk::Device& device;
		vk::SwapchainKHR* swapchain;
		std::vector<vkUtil::SwapchainFrame> swapchainFrames;
		vk::Format swapchainFormat;
		vk::Extent2D swapchainExtent;

		// command buffer:
		std::vector<vk::CommandBuffer> commandBuffers;

		uint32_t currenImageIndex;
		bool isFrameStarted;

		// resizing window:
		void recreate_swapchain();
	};
}
