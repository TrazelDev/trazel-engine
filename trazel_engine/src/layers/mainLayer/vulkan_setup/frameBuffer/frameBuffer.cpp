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
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = inputChunk.renderpass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = inputChunk.swapchainExtent.width;
		framebufferInfo.height = inputChunk.swapchainExtent.height;
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