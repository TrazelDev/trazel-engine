#include "pch.h"
#include "mainLyaer.h"
#include "vulkan_setup/pipeline/pipeline.h"
#include "vulkan_setup/frameBuffer/frameBuffer.h"
#include "vulkan_setup/commands/commands.h"
#include "vulkan_setup/synchronization/sync.h"
#include "vulkan_setup/DescriptorPool/DescriptorPool.h"
#include "vulkan_setup/vulkan_logging/vulkan_logging.h"
#include "vulkan_setup/instance/instance.h"
#include "vulkan_setup/device/device.h"


namespace tze
{
	mainLyaer::~mainLyaer()
	{
		device.waitIdle();
		//model.reset();


		device.destroyCommandPool(commandPool);

		device.destroyPipeline(pipeline);
		device.destroyPipelineLayout(layout);
		device.destroyRenderPass(renderpass);

		for (vkUtil::SwapchainFrame frame : swapchainFrames)
		{
			device.destroyImageView(frame.imageView);
			device.destroyFramebuffer(frame.frameBuffer);
			device.destroyFence(frame.inFlight);
			device.destroySemaphore(frame.imageAvailable);
			device.destroySemaphore(frame.renderFinished);
		}

		device.destroySwapchainKHR();
		device.destroyDescriptorPool();
		device.destroy();


		instance.destroySurfaceKHR(surface);

#ifndef Client_MODE
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
#endif


		instance.destroy();
	}


	void mainLyaer::onAttach()
	{
		makeInstance();
		makeDevice();
		loadGameObj();
		//loadModel();
		makePipeline();
		makeCommands();
		createSemaphoresAndfence();
	}


	void mainLyaer::makeInstance()
	{
		instance = vkInit::make_instance(title);
		dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

#ifndef Client_MODE
		debugMessenger = vkInit::make_debug_messenger(instance, dldi);
#endif

		VkSurfaceKHR c_stayle_surface;
		if (glfwCreateWindowSurface(instance, window, nullptr, &c_stayle_surface) != VK_SUCCESS)
		{
#ifndef Client_MODE
			TZE_ENGINE_ERR("failed to abstract the glfw surface for Vulkan.\n");
#endif
		}
		else
		{
#ifndef Client_MODE
			TZE_ENGINE_INFO("Successfully to abstracted the glfw surface for Vulkan.");
#endif
		}

		surface = c_stayle_surface;
	}


	void mainLyaer::makeDevice()
	{
		physicalDevice = vkInit::choose_physical_device(instance);
		device = vkInit::create_logical_device(physicalDevice, surface);
		std::array<vk::Queue, 2> queues = vkInit::get_queue(physicalDevice, device, surface);
		graphicsQueue = queues[0];
		presentQueue = queues[1];

		bundle = vkInit::create_swapchain(device, physicalDevice, surface, *width, *height);
		swapchain = new vk::SwapchainKHR(bundle.swapchain);
		swapchainFrames = bundle.frames;
		swapchainFormat = bundle.format;
		swapchainExtent = bundle.extent;
		maxFramesInFlight = int(swapchainFrames.size());
		frameNum = 0;
	}


	void mainLyaer::makePipeline()
	{
		vkInit::graphicsPiplineInBundle spesefication = {};
		spesefication.device = device;
		spesefication.vertexFilepath = "C:\\Dev_programmer\\trazel_engine\\trazel_engine\\src\\layers\\mainLayer\\shading\\vertex.spv";
		spesefication.fragmentFilepath = "C:\\Dev_programmer\\trazel_engine\\trazel_engine\\src\\layers\\mainLayer\\shading\\fragment.spv";
		spesefication.swapchainImageFormat = swapchainFormat;
		spesefication.swapchainExtent = swapchainExtent;

		vkInit::graphicsPiplineOutBundle output = vkInit::make_graphics_pipeline(spesefication);

		layout = output.layout;
		renderpass = output.renderpass;
		pipeline = output.pipeline;
	}


	void mainLyaer::makeCommands()
	{
		vkInit::frameBufferInput frameBufferInput;
		frameBufferInput.device = device;
		frameBufferInput.renderpass = renderpass;
		frameBufferInput.swapchainExtent = swapchainExtent;
		vkInit::make_framebuffers(frameBufferInput, swapchainFrames);

		commandPool = vkInit::make_command_pool(device, physicalDevice, surface);

		vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapchainFrames };
		commandBuffer = vkInit::make_command_buffers(commandBufferInput);

		//vkInit::createDescriptorPool(device, descriptorPool);
	}


	void mainLyaer::createSemaphoresAndfence()
	{
		for (vkUtil::SwapchainFrame& frame : swapchainFrames)
		{
			frame.inFlight = vkInit::make_fence(device);
			frame.imageAvailable = vkInit::make_semaphore(device);
			frame.renderFinished = vkInit::make_semaphore(device);
		}
	}


	void mainLyaer::recordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
	{
		vk::CommandBufferBeginInfo beginInfo = {};
		try
		{
			commandBuffer.begin(beginInfo);
		}
		catch (vk::SystemError err)
		{
#ifndef Client_MODE
			TZE_ENGINE_ERR("failed to begin recording command buufer");
#endif
		}

		vk::RenderPassBeginInfo renderpassInfo = {};
		renderpassInfo.renderPass = renderpass;
		renderpassInfo.framebuffer = swapchainFrames[imageIndex].frameBuffer;
		renderpassInfo.renderArea.offset.x = 0;
		renderpassInfo.renderArea.offset.y = 0;
		renderpassInfo.renderArea.extent = swapchainExtent;

		vk::ClearValue clearColor = { std::array<float, 4> {0.1f, 0.1f, 0.1f, 1.0f} };
		renderpassInfo.clearValueCount = 1;
		renderpassInfo.pClearValues = &clearColor;

		commandBuffer.beginRenderPass(&renderpassInfo, vk::SubpassContents::eInline);


		// viewport and scissor:
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = *width;
		viewport.height = *height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, {*width, *height} };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
		renderGameObj(commandBuffer);

		commandBuffer.endRenderPass();

		try
		{
			commandBuffer.end();
		}
		catch (vk::SystemError err)
		{
#ifndef Client_MODE
			TZE_ENGINE_ERR("failed to finish command buffer");
#endif
		}
	}


	void mainLyaer::render()
	{
		device.waitForFences(1, &swapchainFrames[frameNum].inFlight, VK_TRUE, UINT64_MAX);
		device.resetFences(1, &swapchainFrames[frameNum].inFlight);

		uint32_t imageIndex{ device.acquireNextImageKHR(*swapchain, UINT64_MAX, swapchainFrames[frameNum].imageAvailable, nullptr).value };

		vk::CommandBuffer commandBuffer = swapchainFrames[frameNum].commandBuffer;

		commandBuffer.reset();

		recordDrawCommands(commandBuffer, imageIndex);

		vk::SubmitInfo submitInfo = {};

		vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNum].imageAvailable };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vk::Semaphore signalSemaphores[] = { swapchainFrames[frameNum].renderFinished };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		try {
			graphicsQueue.submit(submitInfo, swapchainFrames[frameNum].inFlight);
		}
		catch (vk::SystemError err) {
#ifndef Client_MODE
			TZE_ENGINE_ERR("failed to submit draw command buffer!");
#endif
		}


		vk::PresentInfoKHR presentInfo = {};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		vk::SwapchainKHR swapChains[] = { *swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;


		////////////////////////
		// uint32_t frameNum32_t = frameNum;
		// VkResult result1 = vkAcquireNextImageKHR
		// (
		// 	device,
		// 	*swapchain,
		// 	std::numeric_limits<uint64_t>::max(),
		// 	swapchainFrames[frameNum].imageAvailable,  // must be a not signaled semaphore
		// 	swapchainFrames[frameNum].inFlight,
		// 	&frameNum32_t
		// );
		// if (result1 == VK_ERROR_OUT_OF_DATE_KHR)
		// {
		// 	recreate_swapchain();
		// }
		////////////////////////////////

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mainWindow->wasWindowResized())
		{
			mainWindow->resetWindowResizedFlag();
			recreate_swapchain();
			return;
		}
		frameNum = (frameNum + 1) % maxFramesInFlight;
		result = presentQueue.presentKHR(presentInfo);

		//const auto a = VkPresentInfoKHR(presentInfo);
		//result1 = vkQueuePresentKHR(presentQueue, &a);
	}


	void mainLyaer::onUpdate()
	{
		render();
		calculateFrameRate();
	}


	void mainLyaer::calculateFrameRate()
	{
		currentTime = glfwGetTime();
		double delta = currentTime - lastTime;

		if (delta >= 1)
		{
			int framerate{ std::max(1, int(numFrames / delta)) };
			std::stringstream title;
			title << "running at " << framerate << " fps.";
			glfwSetWindowTitle(window, title.str().c_str());
			lastTime = currentTime;
			numFrames = -1;
			frameTime = float(1000.0 / framerate);
		}

		numFrames++;
	}


	void make
	(
		std::vector<vkUtil::model::vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top,
		glm::vec3 colorLeft,
		glm::vec3 colroRight,
		glm::vec3 colorTop
	)
	{
		if (depth <= 0) {
			vertices.push_back({ top   ,colorTop });
			vertices.push_back({ right ,colroRight });
			vertices.push_back({ left  ,colorLeft });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			make(vertices, depth - 1, left, leftRight, leftTop, colorLeft, colroRight, colorTop);
			make(vertices, depth - 1, leftRight, right, rightTop, colorLeft, colroRight, colorTop);
			make(vertices, depth - 1, leftTop, rightTop, top, colorLeft, colroRight, colorTop);
		}
	}

	void mainLyaer::recreate_swapchain()
	{
		while (width == 0 || height == 0)
		{
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device);
		bundle = vkInit::create_swapchain(device, physicalDevice, surface, *width, *height , swapchain);
		swapchain = new vk::SwapchainKHR(bundle.swapchain);
		swapchainFrames = bundle.frames;
		swapchainFormat = bundle.format;
		swapchainExtent = bundle.extent;

		//makePipeline();
		device.destroyCommandPool(commandPool);
		makeCommands();
		createSemaphoresAndfence();
	}


	void mainLyaer::loadGameObj()
	{
		std::vector<vkUtil::model::vertex> vertices
		{
			{{0.5f, -0.5f }},
			{{0.0f, 0.5f  }},
			{{-0.5f, -0.5f}}
		};

		std::shared_ptr<vkUtil::model> model = std::make_shared<vkUtil::model>(physicalDevice, device, vertices);

		auto triangle = gameObject::createGameObj();
		triangle.model = model;
		triangle.color = { 0.8f, 0.0f, 0.1f };
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.translation.y = 0.0f;
		triangle.transform2d.scale = { 2.0f, 0.5f };
		triangle.transform2d.rotation = -0.25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}


	void mainLyaer::renderGameObj(vk::CommandBuffer commandBuffer)
	{
		for (auto& obj : gameObjects)
		{
			obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0005f, glm::two_pi<float>());

			vkInit::simplePushConstantData push;
			push.offset = obj.transform2d.translation;
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f };
			push.transform = obj.transform2d.mat2();

			vkCmdPushConstants
			(
				commandBuffer,
				layout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(vkInit::simplePushConstantData),
				&push
			);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}


	/*
	void mainLyaer::loadModel()
	{
		std::vector<vkUtil::model::vertex> vertices
		{
			// {{1.0f, -1.0f }, {1.0, 0.0, 0.5}},
			// {{0.0f, 1.0f  }, {1.0, 0.5, 0.0}},
			// {{-1.0f, -1.0f}, {1.0, 0.5, 0.5}}
			{{0.5f, -0.5f }},
			{{0.0f, 0.5f  }},
			{{-0.5f, -0.5f}}

		};

		// make(vertices, 7, { -1.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, -1.0f }, { 0.0, 0.5, 1.0 }, { 0.5, 0.0, 1.0 }, { 0.0, 0.0, 1.0 });
		// 
		// vertices.push_back({ {0.0f, 1.0f   } ,{0.0, 0.5, 1.0} });
		// vertices.push_back({ {-1.0f, 1.0f  } ,{0.5, 0.0, 1.0} });
		// vertices.push_back({ {-1.0f, -1.0f } ,{0.0, 0.0, 1.0} });
		// make(vertices, 7, { -1.0f, -1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0, 1.0, 1.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 });
		// 
		// vertices.push_back({ {1.0f, 1.0f } ,{0.0, 1.0, 1.0} });
		// vertices.push_back({ {0.0f, 1.0f } ,{1.0, 1.0, 0.0} });
		// vertices.push_back({ {1.0f, -1.0f} ,{0.0, 1.0, 0.0} });
		// make(vertices, 7, { 1.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0, 0.5, 0.5 }, { 1.0, 0.5, 0.0 }, { 1.0, 0.0, 0.5 });
		// 
		model = std::unique_ptr<vkUtil::model>(new vkUtil::model{ physicalDevice, device, vertices });
	}
	*/
}