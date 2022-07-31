#include "pch.h"
#include "commands.h"


vk::CommandPool vkInit::make_command_pool(vk::Device& device, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface)
{
	vkUtil::queueFamiltyIndicies queueFamilyIndices = vkUtil::findQueueFamilies(physicalDevice, surface);

	vk::CommandPoolCreateInfo poolInfo = {};
	poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	try
	{
		return device.createCommandPool(poolInfo);
	}
	catch (vk::SystemError err)
	{
		#ifdef DEBUG_MODE
		std::cout << "failed to create command pool" << std::endl;
		#endif
		return nullptr;
	}
}


vk::CommandBuffer vkInit::make_command_buffers(commandBufferInputChunk inputChunk)
{
	vk::CommandBufferAllocateInfo alloceInfo = {};
	alloceInfo.commandPool = inputChunk.commandpool;
	alloceInfo.level = vk::CommandBufferLevel::ePrimary;
	alloceInfo.commandBufferCount = 1;

	for (int i = 0; i < inputChunk.frames.size(); i++)
	{
		try
		{
			inputChunk.frames[i].commandBuffer = inputChunk.device.allocateCommandBuffers(alloceInfo)[0];

			#ifndef Client_MODE
			std::cout << "allocated command buffer for frame " << i << std::endl;
			#endif
		}
		catch (vk::SystemError err)
		{
			#ifndef Client_MODE
			std::cout << "failed to allocate command buffer for frame " << i << std::endl;
			#endif
		}
	}

	try
	{
		vk::CommandBuffer commandBuffer = inputChunk.device.allocateCommandBuffers(alloceInfo)[0];

		#ifndef Client_MODE
		std::cout << "allocated main command buffer" << std::endl;
		#endif

		return commandBuffer;
	}
	catch (vk::SystemError err)
	{
		#ifndef Client_MODE
		std::cout << "failed tp allocate main command buffer" << std::endl;
		#endif
		return nullptr;
	}
}

