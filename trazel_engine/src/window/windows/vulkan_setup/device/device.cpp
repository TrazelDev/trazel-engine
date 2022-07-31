#include "pch.h"
#include "device.h"

void vkInit::log_device_properties(const vk::PhysicalDevice& device)
{
	vk::PhysicalDeviceProperties proprties = device.getProperties();

	std::cout << "device name: " << proprties.deviceName << '\n';

	std::cout << "device type: ";
	switch (proprties.deviceType)
	{
	case (vk::PhysicalDeviceType::eCpu):
		std::cout << "CPU\n";
		break;

	case (vk::PhysicalDeviceType::eDiscreteGpu):
		std::cout << "Discrte GPU\n";
		break;

	case (vk::PhysicalDeviceType::eIntegratedGpu):
		std::cout << "Integrated GPU\n";
		break;

	case (vk::PhysicalDeviceType::eVirtualGpu):
		std::cout << "Virtuel GPU\n";
		break;

	default:
		std::cout << "Other\n";
	}
}

bool vkInit::checkDeviceExtentionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtentions)
{
	// check if the given device can satisfy a list requested device extentions
	std::set<std::string> requiredExtentions(requestedExtentions.begin(), requestedExtentions.end());

	#ifdef DEBUG_MODE
	std::cout << "device can support extentions:\n";
	#endif

	for (vk::ExtensionProperties& extention : device.enumerateDeviceExtensionProperties())
	{
		#ifdef DEBUG_MODE
		std::cout << "\t\"" << extention.extensionName << "\"\n";
		#endif

		// remove this from the list required extentions ( set checks for equality automaticlly )
		requiredExtentions.erase(extention.extensionName);
	}

	// if the set is emepty then all requirments have been satisfied
	return requiredExtentions.empty();
}


bool vkInit::is_suitable(const vk::PhysicalDevice& device)
{
	#ifdef DEBUG_MODE
	std::cout << "checking if the device is suitible\n";
	#endif

	// a device is suitable if it can present to the screen, ie support the swapchaing extention
	const std::vector<const char*> requestedExtentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	#ifdef DEBUG_MODE
	std::cout << "we are requesting device extentions:\n";
	for (const char* extention : requestedExtentions)
	{
		std::cout << "\t\"" << extention << "\"\n";
	}
	#endif

	if (bool extentionSupported = checkDeviceExtentionSupport(device, requestedExtentions))
	{
		#ifdef DEBUG_MODE
		std::cout << "device can suppotrt the requested extentions!\n";
		#endif
	}
	else
	{
		#ifdef DEBUG_MODE
		std::cout << "device can't suppotrt the requested extentions!\n";
		#endif
		return false;
	}
	return true;
}


vk::PhysicalDevice vkInit::choose_physical_device(vk::Instance& instance)
{
	#ifdef DEBUG_MODE
	std::cout << "Choosing physical device...\n";
	#endif
	std::vector<vk::PhysicalDevice> avalibleDevices = instance.enumeratePhysicalDevices();

	#ifdef DEBUG_MODE
	std::cout << "there are " << avalibleDevices.size() << " physical devices avalible on this system\n";
	#endif

	// checking if the is a suitible device
	for (vk::PhysicalDevice device : avalibleDevices)
	{
		#ifdef DEBUG_MODE
		log_device_properties(device);
		#endif

		if (is_suitable(device))
		{
			return device;
		}
	}
	return nullptr;
}


vk::Device vkInit::create_logical_device(vk::PhysicalDevice& physycalDevice, vk::SurfaceKHR& surface)
{
	vkUtil::queueFamiltyIndicies indices = vkUtil::findQueueFamilies(physycalDevice, surface);
	std::vector<uint32_t> uniqueIndices;
	uniqueIndices.push_back(indices.graphicsFamily.value());
	if (indices.graphicsFamily.value() != indices.presentFamily.value())
	{
		uniqueIndices.push_back(indices.presentFamily.value());
	}
	float queuePriority = 1.0f;

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
	for (uint32_t queueFamilyIndex : uniqueIndices)
	{
		queueCreateInfo.push_back
		(
			vk::DeviceQueueCreateInfo
			(
				vk::DeviceQueueCreateFlags(),
				queueFamilyIndex,
				1,
				&queuePriority
			)
		);
	}

	std::vector<const char*> deviceExtentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

	std::vector<const char*> enabledLayers;
	/*
	if (debug)
	{
		enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
	}
	*/

	vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo
	(
		vk::DeviceCreateFlags(),
		queueCreateInfo.size(), queueCreateInfo.data(),
		enabledLayers.size(), enabledLayers.data(),
		deviceExtentions.size(), deviceExtentions.data(),
		&deviceFeatures
	);

	try
	{
		vk::Device device = physycalDevice.createDevice(deviceInfo);
		/*
		if (debug)
		{
			std::cout << "GPU has been succssfully abstracted!\n";
		}
		*/
		return device;
	}
	catch (vk::SystemError err)
	{
		/*
		if (debug)
		{
			std::cout << "Device creation failed!\n";
		}
		*/
		return nullptr;
	}

}


std::array<vk::Queue, 2> vkInit::get_queue(vk::PhysicalDevice& PhysicalDevice, vk::Device& device, vk::SurfaceKHR& surface)
{
	vkUtil::queueFamiltyIndicies indices = vkUtil::findQueueFamilies(PhysicalDevice, surface); // you are using this function in this funtion and in the one befor it change it in the future
	return
	{
		device.getQueue(indices.graphicsFamily.value(), 0),
		device.getQueue(indices.presentFamily.value(), 0)
	};
}