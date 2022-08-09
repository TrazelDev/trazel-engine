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

mainLyaer::~mainLyaer()
{
	device.waitIdle();


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
	//device.destroyDescriptorPool();
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
	makePipeline();
	finalSetup();
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

	vkInit::SwapchainBundle bundle = vkInit::create_swapchain(device, physicalDevice, surface, width, height);
	swapchain = bundle.swapchain;
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


void mainLyaer::finalSetup()
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

	vk::ClearValue clearColor = { std::array<float, 4> {1.0f, 1.0f, 1.0f, 1.0f} };
	renderpassInfo.clearValueCount = 1;
	renderpassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderpassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
	commandBuffer.draw(3, 1, 0, 0);
	commandBuffer.draw(3, 1, 3, 0);
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

	uint32_t imageIndex{ device.acquireNextImageKHR(swapchain, UINT64_MAX, swapchainFrames[frameNum].imageAvailable, nullptr).value };

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

	vk::SwapchainKHR swapChains[] = { swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	presentQueue.presentKHR(presentInfo);

	frameNum = (frameNum + 1) % maxFramesInFlight;
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