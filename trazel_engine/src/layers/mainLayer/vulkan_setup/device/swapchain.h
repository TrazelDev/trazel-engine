#pragma once
#include "pch.h"
#include "../vulkan_logging/vulkan_logging.h"
#include "../../vulkan_utility/queueFamilies/queueFamilies.h"
#include "../../vulkan_utility/frame.h"

namespace vkInit
{
	struct swapchainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capablities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	struct SwapchainBundle
	{
		vk::SwapchainKHR swapchain;
		std::vector<vkUtil::SwapchainFrame> frames;
		vk::Format format;
		vk::Extent2D extent;
	};

	swapchainSupportDetails query_swapchain_support(vk::PhysicalDevice& device, vk::SurfaceKHR& surface);
	vk::SurfaceFormatKHR choose_swapchain_surface_fromat(std::vector<vk::SurfaceFormatKHR>& formats);
	vk::PresentModeKHR choose_swapchain_presentmode(std::vector<vk::PresentModeKHR>& presentModes);
	vk::Extent2D choose_swapchain_extent(uint32_t& width, uint32_t& height, vk::SurfaceCapabilitiesKHR& capabilities);
	SwapchainBundle create_swapchain(vk::Device& logicalDevice, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface, uint32_t& width, uint32_t& height, vk::SwapchainKHR* oldSwapchain = nullptr);
}


