#pragma once
#include "pch.h"

namespace vkInit
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
	(
		VkDebugUtilsMessageSeverityFlagBitsEXT messangerServerity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);


	vk::DebugUtilsMessengerEXT make_debug_messenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);


	std::vector<std::string> log_transform_bits(vk::SurfaceTransformFlagsKHR bits);


	std::vector<std::string> log_alpha_composite_string(vk::CompositeAlphaFlagsKHR bits);


	std::vector<std::string> log_image_usage_bits(vk::ImageUsageFlags bits);


	std::string log_presentmode(vk::PresentModeKHR presentMode);
}