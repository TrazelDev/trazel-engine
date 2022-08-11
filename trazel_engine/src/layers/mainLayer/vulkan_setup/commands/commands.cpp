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
		#ifndef Client_MODE
		TZE_ENGINE_ERR("failed to create command pool");
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
			TZE_ENGINE_INFO("allocated command buffer for frame ");
			#endif
		}
		catch (vk::SystemError err)
		{
			#ifndef Client_MODE
			TZE_ENGINE_ERR("failed to allocate command buffer for frame ");
			#endif
		}
	}

	try
	{
		vk::CommandBuffer commandBuffer = inputChunk.device.allocateCommandBuffers(alloceInfo)[0];

		#ifndef Client_MODE
		TZE_ENGINE_INFO("allocated main command buffer");
		#endif

		return commandBuffer;
	}
	catch (vk::SystemError err)
	{
		#ifndef Client_MODE
		TZE_ENGINE_ERR("failed tp allocate main command buffer");
		#endif
		return nullptr;
	}
}
