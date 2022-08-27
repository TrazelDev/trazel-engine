#pragma once
#include "../layer.h"
#include "vulkan_setup/device/swapchain.h"
#include "vulkan_utility/model/model.h"
#include "gameObject/gameObject.h"

namespace tze
{
	class mainLyaer : layer
	{
	public:
		mainLyaer(GLFWwindow* Window, const char* Title, uint32_t* Width, uint32_t* Height, const std::unique_ptr<windowsWindow>& MainWindow)
		{
			window = Window;
			title = Title;
			width = Width;
			height = Height;
			mainWindow = MainWindow.get();
		}
		~mainLyaer();

		virtual void onAttach() override;
		virtual void onDetach() override {};
		virtual void onUpdate() override;
		virtual void onEvent() override {};

	private:
		windowsWindow* mainWindow;

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
		vk::SwapchainKHR* swapchain;
		std::vector<vkUtil::SwapchainFrame> swapchainFrames;
		vk::Format swapchainFormat;
		vk::Extent2D swapchainExtent;
		vkInit::SwapchainBundle bundle;

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

		// model realted variables:
		//std::unique_ptr<vkUtil::model> model;
		std::vector<gameObject> gameObjects;

		// getting results:
		vk::Result result;

		// instance setup:
		void makeInstance();

		// device setup:
		void makeDevice();

		// pipeline setup:
		void makePipeline();

		// loading game object:
		//void loadModel();
		void loadGameObj();

		void makeCommands();

		void createSemaphoresAndfence();

		void recordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);

		void renderGameObj(vk::CommandBuffer commandBuffer);
		void render();
		void calculateFrameRate();

		// resizing window:
		void recreate_swapchain();
	};
}
