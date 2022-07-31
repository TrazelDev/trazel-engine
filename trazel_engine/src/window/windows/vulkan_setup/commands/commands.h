#pragma once
#include "pch.h"
#include "../../vulkan_utility/queueFamilies/queueFamilies.h"
#include "../../vulkan_utility/frame.h"

namespace vkInit
{
	struct commandBufferInputChunk
	{
		vk::Device device;
		vk::CommandPool commandpool;
		std::vector<vkUtil::SwapchainFrame>& frames;
	};

	vk::CommandPool make_command_pool(vk::Device& device, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface);
	vk::CommandBuffer make_command_buffers(commandBufferInputChunk inputChunk);
}