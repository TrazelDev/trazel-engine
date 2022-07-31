#include "pch.h"
#include "swapchain.h"

vkInit::swapchainSupportDetails vkInit::query_swapchain_support(vk::PhysicalDevice& device, vk::SurfaceKHR& surface)
{
	swapchainSupportDetails support;

	support.capablities = device.getSurfaceCapabilitiesKHR(surface);

	#ifdef DEBUG_MODE
	std::cout << "swapChain can supported the following surface capabilities:\n";

	std::cout << "\tminimum image cout: " << support.capablities.minImageCount << '\n';
	std::cout << "\tmaximum image cout: " << support.capablities.maxImageCount << '\n';

	std::cout << "\tcurrent extent: \n";
	std::cout << "\t\twidth: " << support.capablities.currentExtent.width << '\n';
	std::cout << "\t\theight: " << support.capablities.currentExtent.height << '\n';

	std::cout << "\tminimum supported extent:\n";
	std::cout << "\t\twidth: " << support.capablities.minImageExtent.width << '\n';
	std::cout << "\t\theight: " << support.capablities.minImageExtent.height << '\n';

	std::cout << "\maximum supported extent:\n";
	std::cout << "\t\twidth: " << support.capablities.maxImageExtent.width << '\n';
	std::cout << "\t\theight: " << support.capablities.maxImageExtent.height << '\n';

	std::cout << "\tmaximum image array layers: " << support.capablities.maxImageArrayLayers << '\n';

	std::cout << "\tsupported transform:\n";
	std::vector<std::string> stringList = log_transform_bits(support.capablities.supportedTransforms);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << '\n';
	}

	std::cout << "\tcurrent transform:\n";
	stringList = log_transform_bits(support.capablities.currentTransform);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << '\n';
	}

	std::cout << "\tsupported alpha operations:\n";
	stringList = log_alpha_composite_string(support.capablities.supportedCompositeAlpha);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << '\n';
	}

	std::cout << "\tsupported image usage:\n";
	stringList = log_image_usage_bits(support.capablities.supportedUsageFlags);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << "\n";
	}
	#endif 


	support.formats = device.getSurfaceFormatsKHR(surface);
	#ifdef DEBUG_MODE
	for (vk::SurfaceFormatKHR supportedFormat : support.formats)
	{
		std::cout << "supported pixel format: " << vk::to_string(supportedFormat.format) << '\n';
		std::cout << "supported color space: " << vk::to_string(supportedFormat.colorSpace) << '\n';
	}
	#endif 


	support.presentModes = device.getSurfacePresentModesKHR(surface);
	#ifdef DEBUG_MODE
	for (vk::PresentModeKHR presentMode : support.presentModes)
	{
		std::cout << '\t' << log_presentmode(presentMode) << '\n';
	}
	#endif 

	return support;
}


vk::SurfaceFormatKHR vkInit::choose_swapchain_surface_fromat(std::vector<vk::SurfaceFormatKHR>& formats)
{
	for (vk::SurfaceFormatKHR format : formats)
	{
		if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			return format;
		}
	}

	return formats[0];
}


vk::PresentModeKHR vkInit::choose_swapchain_presentmode(std::vector<vk::PresentModeKHR>& presentModes)
{
	for (vk::PresentModeKHR presentMode : presentModes)
	{
		if (presentMode == vk::PresentModeKHR::eMailbox)
		{
			return presentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}


vk::Extent2D vkInit::choose_swapchain_extent(uint32_t& width, uint32_t& height, vk::SurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		vk::Extent2D extent = { width, height };

		extent.width = std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, width));
		extent.height = std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, height));

		return extent;
	}
}


vkInit::SwapchainBundle vkInit::create_swapchain(vk::Device& logicalDevice, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface, uint32_t& width, uint32_t& height)
{
	swapchainSupportDetails support = query_swapchain_support(physicalDevice, surface);

	vk::SurfaceFormatKHR format = choose_swapchain_surface_fromat(support.formats);

	vk::PresentModeKHR presentMode = choose_swapchain_presentmode(support.presentModes);

	vk::Extent2D extent = choose_swapchain_extent(width, height, support.capablities);

	uint32_t imageCount = std::min(support.capablities.maxImageCount, support.capablities.minImageCount + 1);

	vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR
	(
		vk::SwapchainCreateFlagsKHR(), surface, imageCount, format.format, format.colorSpace,
		extent, 1, vk::ImageUsageFlagBits::eColorAttachment
	);

	vkUtil::queueFamiltyIndicies indices = vkUtil::findQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyindices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily.value() != indices.presentFamily.value())
	{
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyindices;
	}
	else
	{
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}

	createInfo.preTransform = support.capablities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

	SwapchainBundle bundle;
	try
	{
		bundle.swapchain = logicalDevice.createSwapchainKHR(createInfo);
	}
	catch (vk::SystemError err)
	{
		throw std::runtime_error("failed to create swapchain");
	}

	std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
	bundle.frames.resize(images.size());

	for (size_t i = 0; i < images.size(); i++)
	{
		vk::ImageViewCreateInfo createInfo = {};
		createInfo.image = images[i];
		createInfo.viewType = vk::ImageViewType::e2D;
		createInfo.components.r = vk::ComponentSwizzle::eIdentity;
		createInfo.components.g = vk::ComponentSwizzle::eIdentity;
		createInfo.components.b = vk::ComponentSwizzle::eIdentity;
		createInfo.components.a = vk::ComponentSwizzle::eIdentity;
		createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		createInfo.format = format.format;

		bundle.frames[i].image = images[i];
		bundle.frames[i].imageView = logicalDevice.createImageView(createInfo);
	}
	bundle.format = format.format;
	bundle.extent = extent;

	return bundle;
}
