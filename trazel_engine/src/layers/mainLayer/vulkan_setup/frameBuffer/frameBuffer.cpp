#include "pch.h"
#include "frameBuffer.h"

void vkInit::make_framebuffers(frameBufferInput& inputChunk, std::vector<vkUtil::SwapchainFrame>& frames)
{
	for (int i = 0; i < frames.size(); i++)
	{
		std::vector<vk::ImageView> attachments
		{
			frames[i].imageView
		};

		vk::FramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.width = inputChunk.swapchainExtent.width;
		framebufferInfo.height = inputChunk.swapchainExtent.height;;
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = inputChunk.renderpass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.layers = 1;

		try
		{
			frames[i].frameBuffer = inputChunk.device.createFramebuffer(framebufferInfo);
		}
		catch (vk::SystemError err)
		{
			#ifndef Client_MODE
			TZE_ENGINE_ERR("failed to create framebuffer for frame number ",  i);
			#endif
		}
	}
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, vk::PhysicalDevice& device) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void vkInit::createBuffer
(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	vk::Device& device,
	vk::PhysicalDevice& physicalDevice,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory
)
{
	VkBufferCreateInfo bufferInfo = {};
	// bufferInfo.flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
	  throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
	  throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}


