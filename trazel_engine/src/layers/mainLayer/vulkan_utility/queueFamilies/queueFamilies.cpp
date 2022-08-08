#include "pch.h"
#include "queueFamilies.h"

vkUtil::queueFamiltyIndicies vkUtil::findQueueFamilies(vk::PhysicalDevice& device, vk::SurfaceKHR& surface)
{
	queueFamiltyIndicies indices;

	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

	#ifdef DEBUG_MODE
	std::cout << "System can support " << queueFamilies.size() << " queue families.\n";
	#endif

	int i = 0;
	for (vk::QueueFamilyProperties queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			indices.graphicsFamily = i;

			#ifdef DEBUG_MODE
			std::cout << "Queue Family " << i << " is suitible for graphics\n";
			#endif
		}

		if (device.getSurfaceSupportKHR(i, surface))
		{
			indices.presentFamily = i;

			#ifdef DEBUG_MODE
			std::cout << "Queue Family " << i << " is suitible for presenting\n";
			#endif
		}

		if (indices.isComlete())
		{
			break;
		}

		i++;
	}

	return indices;
}

