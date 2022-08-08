#pragma once
#include "pch.h"

namespace vkUtil
{
	struct SwapchainFrame
	{
		vk::Image image;
		vk::ImageView imageView;
		vk::Framebuffer frameBuffer;
		vk::CommandBuffer commandBuffer;
	};
}