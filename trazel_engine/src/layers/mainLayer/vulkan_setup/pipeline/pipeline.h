#pragma once
#include "../vulkan_logging/vulkan_logging.h"
#include "../../vulkan_utility/shaders/shaders.h"

namespace vkInit
{
	struct simplePushConstantData
	{
		glm::mat2 transform{1.0f};
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	struct graphicsPiplineInBundle
	{
		vk::Device device;
		std::string vertexFilepath;
		std::string fragmentFilepath;
		vk::Extent2D swapchainExtent;
		vk::Format swapchainImageFormat;
	};

	struct graphicsPiplineOutBundle
	{
		vk::PipelineLayout layout;
		vk::RenderPass renderpass;
		vk::Pipeline pipeline;
	};

	vk::PipelineLayout make_pipeline_layout(vk::Device& device);
	vk::RenderPass make_renderpass(vk::Device& device, vk::Format& swapchainImageFormat);

	graphicsPiplineOutBundle make_graphics_pipeline(graphicsPiplineInBundle& specification);
}
