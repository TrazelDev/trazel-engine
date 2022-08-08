#include "pch.h"
#include "pipeline.h"
vk::PipelineLayout vkInit::make_pipeline_layout(vk::Device& device)
{
	vk::PipelineLayoutCreateInfo layoutInfo;
	layoutInfo.flags = vk::PipelineLayoutCreateFlags();
	layoutInfo.setLayoutCount = 0;
	layoutInfo.pushConstantRangeCount = 0;

	try
	{
		return device.createPipelineLayout(layoutInfo);
	}
	catch (vk::SystemError err)
	{
		#ifndef Client_MODE
		TZE_ENGINE_ERR("Failed to create pipeline layout!");
		#endif
	}

}


vk::RenderPass vkInit::make_renderpass(vk::Device& device, vk::Format& swapchainImageFormat)
{
	vk::AttachmentDescription colorAttachment = {};
	colorAttachment.flags = vk::AttachmentDescriptionFlags();
	colorAttachment.format = swapchainImageFormat;
	colorAttachment.samples = vk::SampleCountFlagBits::e1;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::SubpassDescription subpass = {};
	subpass.flags = vk::SubpassDescriptionFlags();
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	vk::RenderPassCreateInfo renderpassInfo = {};
	renderpassInfo.flags = vk::RenderPassCreateFlags();
	renderpassInfo.attachmentCount = 1;
	renderpassInfo.pAttachments = &colorAttachment;
	renderpassInfo.subpassCount = 1;
	renderpassInfo.pSubpasses = &subpass;

	try
	{
		return device.createRenderPass(renderpassInfo);
	}
	catch (vk::SystemError err)
	{
		#ifdef DEBUG_MODE
		TZE_ENGINE_ERR("failed to create renderpass!");
		#endif
	}
}


vkInit::graphicsPiplineOutBundle vkInit::make_graphics_pipeline(vkInit::graphicsPiplineInBundle& specification)
{
	vk::GraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.flags = vk::PipelineCreateFlags();

	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

	// vertext input:
	//vk::VertexInputAttributeDescription vertexInputParamter;
	//vertexInputParamter.binding = 0;
	//vertexInputParamter.location = 0;
	//vertexInputParamter.format = VK_FORMAT_R8G8B8A8_UNORM;

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	pipelineInfo.pVertexInputState = &vertexInputInfo;

	// input assembly:
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
	inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;

	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

	// vertex shader:
	#ifdef DEBUG_MODE
	std::cout << "Create vertex shader module" << std::endl;
	#endif
	vk::ShaderModule vertexShader = vkUtil::createModule(specification.vertexFilepath, specification.device);
	vk::PipelineShaderStageCreateInfo vertexShaderInfo = {};
	vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
	vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertexShaderInfo.module = vertexShader;
	vertexShaderInfo.pName = "main";
	shaderStages.push_back(vertexShaderInfo);

	// viewport and scissor:
	vk::Viewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = specification.swapchainExtent.width;
	viewport.height = specification.swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vk::Rect2D scissor = {};
	scissor.offset.x = 0.0f;
	scissor.offset.y = 0.0f;
	scissor.extent = specification.swapchainExtent;
	vk::PipelineViewportStateCreateInfo viewportState = {};
	viewportState.flags = vk::PipelineViewportStateCreateFlags();
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	pipelineInfo.pViewportState = &viewportState;

	// resterizer:
	vk::PipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = vk::PolygonMode::eFill;
	rasterizer.lineWidth = 1.0;
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eClockwise;
	rasterizer.depthBiasEnable = VK_FALSE;

	pipelineInfo.pRasterizationState = &rasterizer;

	// fragment shader:
	#ifdef DEBUG_MODE
	std::cout << "Create fragment shader module" << std::endl;
	#endif

	vk::ShaderModule fragmentShader = vkUtil::createModule(specification.fragmentFilepath, specification.device);
	vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
	fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
	fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragmentShaderInfo.module = fragmentShader;
	fragmentShaderInfo.pName = "main";
	shaderStages.push_back(fragmentShaderInfo);

	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();

	// multysampling:
	vk::PipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

	pipelineInfo.pMultisampleState = &multisampling;

	// color blend:
	vk::PipelineColorBlendAttachmentState colorBlentAttachment = {};
	colorBlentAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlentAttachment.blendEnable = VK_FALSE;
	vk::PipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
	colorBlending.logicOp = vk::LogicOp::eCopy;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlentAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	pipelineInfo.pColorBlendState = &colorBlending;

	// pipeline layout:
	#ifdef DEBUG_MODE
	std::cout << "Create pipeline layout" << std::endl;
	#endif
	vk::PipelineLayout layout = make_pipeline_layout(specification.device);
	pipelineInfo.layout = layout;

	// renderpass:
	#ifdef DEBUG_MODE
	std::cout << "Create renderpass" << std::endl;
	#endif
	vk::RenderPass renderpass = make_renderpass(specification.device, specification.swapchainImageFormat);
	pipelineInfo.renderPass = renderpass = renderpass;

	// extra stuff:
	pipelineInfo.basePipelineHandle = nullptr;

	// create pipeline:
	#ifdef DEBUG_MODE
	std::cout << "Create graphics pipeline" << std::endl;
	#endif

	
	vk::Pipeline graphicsPipeline;
	try
	{
		graphicsPipeline = (specification.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
	}
	catch (vk::SystemError)
	{
		#ifdef DEBUG_MODE
		TZE_ENGINE_ERR("failed to create graphics pipeline!");
		#endif
	}
	specification.device.destroyShaderModule(vertexShader);
	specification.device.destroyShaderModule(fragmentShader);


	graphicsPiplineOutBundle output = {};
	output.layout = layout;
	output.renderpass = renderpass;
	output.pipeline = graphicsPipeline;
	return output;
}
