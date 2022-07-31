#include "pch.h"
#include "windowsWindow.h"
#include "vulkan_setup/vulkan_logging/vulkan_logging.h"

windowsWindow::windowsWindow(int Width, int Height, const char* Title) : width(Width), height(Height), title(Title)
{
	buildGLFWWindow(width, height);
	makeInstance();
	makeDevice();
	makePipeline();
	finalSetup();
}
windowsWindow::~windowsWindow()
{
	device.waitIdle();

	device.destroyFence(inFlightFence);
	device.destroySemaphore(imageAvailable);
	device.destroySemaphore(renderFinished);

	device.destroyCommandPool(commandPool);

	device.destroyPipeline(pipeline);
	device.destroyPipelineLayout(layout);
	device.destroyRenderPass(renderpass);

	for (vkUtil::SwapchainFrame frame : swapchainFrames)
	{
		device.destroyImageView(frame.imageView);
		device.destroyFramebuffer(frame.frameBuffer);
	}

	device.destroySwapchainKHR();
	device.destroy();

	instance.destroySurfaceKHR(surface);

	#ifdef DEBUG_MODE
	instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	#endif

	instance.destroy();

	glfwTerminate();
}


void windowsWindow::buildGLFWWindow(int width, int height)
{
	// init glfw
	glfwInit();

	// no default rendering client vulkan will be hooked up later
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// resizing break swapchain will be disabled for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE /* can be changed to true in the future */);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);   // you need to check if this procces succseeded later
}


void windowsWindow::makeInstance()
{
	instance = vkInit::make_instance(title);
	dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

	#ifdef DEBUG_MODE
	debugMessenger = vkInit::make_debug_messenger(instance, dldi);
	#endif

	VkSurfaceKHR c_stayle_surface;
	if (glfwCreateWindowSurface(instance, window, nullptr, &c_stayle_surface) != VK_SUCCESS)
	{
		#ifdef DEBUG_MODE
		TZE_ENGINE_ERR("failed to abstract the glfw surface for Vulkan.\n");
		#endif
	}
	else
	{
		#ifdef DEBUG_MODE
		TZE_ENGINE_INFO("Successfully to abstracted the glfw surface for Vulkan.\n");
		#endif
	}

	surface = c_stayle_surface;
}


void windowsWindow::makeDevice()
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
}


void windowsWindow::makePipeline()
{
	vkInit::graphicsPiplineInBundle spesefication = {};
	spesefication.device = device;
	spesefication.vertexFilepath = "C:\\Dev_programmer\\trazel_engine\\trazel_engine\\src\\window\\windows\\shading\\vertex.spv";
	spesefication.fragmentFilepath = "C:\\Dev_programmer\\trazel_engine\\trazel_engine\\src\\window\\windows\\shading\\fragment.spv";
	spesefication.swapchainImageFormat = swapchainFormat;
	spesefication.swapchainExtent = swapchainExtent;

	vkInit::graphicsPiplineOutBundle output = vkInit::make_graphics_pipeline(spesefication);

	layout = output.layout;
	renderpass = output.renderpass;
	pipeline = output.pipeline;
}


void windowsWindow::finalSetup()
{
	vkInit::frameBufferInput frameBufferInput;
	frameBufferInput.device = device;
	frameBufferInput.renderpass = renderpass;
	frameBufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(frameBufferInput, swapchainFrames);

	commandPool = vkInit::make_command_pool(device, physicalDevice, surface);

	vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapchainFrames };
	commandBuffer = vkInit::make_command_buffers(commandBufferInput);

	inFlightFence = vkInit::make_fence(device);
	imageAvailable = vkInit::make_semaphore(device);
	renderFinished = vkInit::make_semaphore(device);


}


void windowsWindow::recordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
{
	vk::CommandBufferBeginInfo beginInfo = {};
	try
	{
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err)
	{
		#ifdef DEBUG_MODE
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
		#ifdef DEBUG_MODE
		TZE_ENGINE_ERR("failed to finish command buffer");
		#endif
	}
}


void windowsWindow::render()
{
	device.waitForFences(1, &inFlightFence, VK_TRUE, UINT64_MAX);
	device.resetFences(1, &inFlightFence);

	uint32_t imageIndex{ device.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailable, nullptr).value };

	vk::CommandBuffer commandBuffer = swapchainFrames[imageIndex].commandBuffer;

	commandBuffer.reset();

	recordDrawCommands(commandBuffer, imageIndex);

	vk::SubmitInfo submitInfo = {};

	vk::Semaphore waitSemaphores[] = { imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vk::Semaphore signalSemaphores[] = { renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	try {
		graphicsQueue.submit(submitInfo, inFlightFence);
	}
	catch (vk::SystemError err) {
		#ifdef DEBUG_MODE
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
}


void windowsWindow::run()
{
	//ImGui::Text("Hello, world %d", 123);
	//ImGui_ImplVulkanH_Window g_MainWindowData;
	//ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
	
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO io = ImGui::GetIO(); 
	//(void)io;
	//ImGui::StyleColorsDark();
	


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		render();
		calculateFrameRate();
	}
}


void windowsWindow::calculateFrameRate()
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

/*
void init_imgui()
{
	//1: create descriptor pool for IMGUI
	// the size of the pool is very oversize, but it's copied from imgui demo itself.
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	vkCreateDescriptorPool(_device, &pool_info, nullptr, &imguiPool);


	// 2: initialize imgui library

	//this initializes the core structures of imgui
	ImGui::CreateContext();

	//this initializes imgui for SDL
	ImGui_ImplGlfw_InitForVulkan(window);

	//this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = physicalDevice;
	init_info.Device = device;
	init_info.Queue = graphicsQueue;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplVulkan_Init(&init_info, renderPass);
	

	//execute a gpu command to upload imgui font textures
	immediate_submit([&](VkCommandBuffer cmd) {
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
		});
	

	//clear font textures from cpu data
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	//add the destroy the imgui created structures
	_mainDeletionQueue.push_function([=]() {

		vkDestroyDescriptorPool(_device, imguiPool, nullptr);
		ImGui_ImplVulkan_Shutdown();
		});
}
*/