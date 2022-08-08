#pragma once
#include "pch.h"
#include "../../vulkan_utility/queueFamilies/queueFamilies.h"

namespace vkInit
{
	void log_device_properties(const vk::PhysicalDevice& device);
	bool checkDeviceExtentionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtentions);
	bool is_suitable(const vk::PhysicalDevice& device);
	vk::PhysicalDevice choose_physical_device(vk::Instance& instance);

	vk::Device create_logical_device(vk::PhysicalDevice& physycalDevice, vk::SurfaceKHR& surface);
	std::array<vk::Queue, 2> get_queue(vk::PhysicalDevice& PhysicalDevice, vk::Device& device, vk::SurfaceKHR& surface);
}


