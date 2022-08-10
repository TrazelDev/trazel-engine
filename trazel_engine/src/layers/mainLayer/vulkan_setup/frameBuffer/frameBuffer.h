#pragma once
#include "../../vulkan_utility/frame.h"

namespace vkInit
{
	struct frameBufferInput
	{
		vk::Device device;
		vk::RenderPass renderpass;
		vk::Extent2D swapchainExtent;
	};

	void make_framebuffers(frameBufferInput& inputChunk, std::vector<vkUtil::SwapchainFrame>& frames);

	void createBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		vk::Device& device,
		vk::PhysicalDevice& physicalDevice,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
}

