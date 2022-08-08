#pragma once
#include "pch.h"

namespace vkUtil
{
	struct queueFamiltyIndicies
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComlete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};


	queueFamiltyIndicies findQueueFamilies(vk::PhysicalDevice& device, vk::SurfaceKHR& surface);
}
