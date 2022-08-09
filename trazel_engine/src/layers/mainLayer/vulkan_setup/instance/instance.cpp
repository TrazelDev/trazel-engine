#include "pch.h"
#include "instance.h"

bool vkInit::supported(std::vector<const char*> extentions, std::vector<const char*>& layers)
{
	std::vector<vk::ExtensionProperties> supportedExtentions = vk::enumerateInstanceExtensionProperties();

	
	#ifdef DEBUG_MODE
	std::cout << "Device can support the follwing extentions:\n";
	for (vk::ExtensionProperties supportedExtention : supportedExtentions)
	{
		std::cout << '\t' << supportedExtention.extensionName << '\n';
	}
	#endif
	

	bool found;
	for (const char* extention : extentions)
	{
		found = false;
		for (vk::ExtensionProperties supportedExtention : supportedExtentions)
		{
			if (strcmp(extention, supportedExtention.extensionName) == 0)
			{
				found = true;
				#ifdef DEBUG_MODE
				std::cout << "Extention \"" << extention << "\" is supported!\n";
				#endif
			}
		}
		if (!found)
		{
			#ifdef DEBUG_MODE
			std::cout << "Extention \"" << extention << "\" is not supported!\n";
			#endif
			return false;
		}
	}

	// check layer support
	std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
	#ifdef DEBUG_MODE
	std::cout << "device can support the follwing layers:\n";
	for(vk::LayerProperties supportedLayer : supportedLayers)
	{
		std::cout << '\t' << supportedLayer.layerName << '\n';
	}
	#endif

	for (const char* layer : layers)
	{
		found = false;
		for (vk::LayerProperties supportedLayer : supportedLayers)
		{
			if (strcmp(layer, supportedLayer.layerName) == 0)
			{
				found = true;
				#ifdef DEBUG_MODE
				std::cout << "Layer \"" << layer << "\" is supported!\n";
				#endif
			}
		}
		if (!found)
		{
			#ifdef DEBUG_MODE
			std::cout << "Layer \"" << layer << "\" is not supported!\n";
			#endif
			return false;
		}
	}
	return true;
}


vk::Instance vkInit::make_instance(const char* title)
{
	uint32_t version{ 0 };
	vkEnumerateInstanceVersion(&version);

	#ifdef DEBUG_MODE
	std::cout << "System can support vulkan variants : " << VK_API_VERSION_VARIANT(version) <<
		", major: " << VK_API_VERSION_MAJOR(version) <<
		", minor: " << VK_API_VERSION_MINOR(version) <<
		", pathc: " << VK_API_VERSION_PATCH(version) << "\n";
	#endif
	version &= ~(0xFFFU);
	version = VK_MAKE_API_VERSION(0, 1, 0, 0);

	vk::ApplicationInfo appInfo = vk::ApplicationInfo
	(
		title,
		version,
		title,
		version,
		version
	);

	uint32_t glfwExtentionCount = 0;
	const char** glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

	std::vector<const char*> extentions(glfwExtentions, glfwExtentions + glfwExtentionCount);
	#ifndef Client_MODE
	extentions.push_back("VK_EXT_debug_utils");
	#endif

	#ifdef DEBUG_MODE
	std::cout << "extentions to be requested : \n";
	for (const char* extentionName : extentions)
	{
		std::cout << "\t\"" << extentionName << "\"\n";
	}
	#endif
	std::vector <const char*> layers;
	#ifndef Client_MODE
	layers.push_back("VK_LAYER_KHRONOS_validation");
	#endif

	if (!supported(extentions, layers))
	{
		return nullptr;
	}

	vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo
	(
		vk::InstanceCreateFlags(),
		&appInfo,
		static_cast<uint32_t>(layers.size()),
		layers.data(),
		static_cast<uint32_t>(extentions.size()),
		extentions.data()
	);

	try
	{
		return vk::createInstance(createInfo);
	}
	catch (vk::SystemError err)
	{
		#ifndef Client_MODE
		TZE_ENGINE_ERR("failed to create instance!");
		#endif
		return nullptr;
	}
}