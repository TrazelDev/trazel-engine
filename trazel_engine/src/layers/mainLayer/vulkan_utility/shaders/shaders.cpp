#include "pch.h"
#include "shaders.h"

std::vector<char> vkUtil::readFile(std::string filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	#ifndef Client_MODE
	if (!file.is_open())
	{
		std::string failedMsg = "failed to load \"" + filename + "\"";
		TZE_ENGINE_ERR(failedMsg);
	}
	#endif
	size_t filesize{ static_cast<size_t>(file.tellg()) };

	std::vector<char> buffer(filesize);
	file.seekg(0);
	file.read(buffer.data(), filesize);

	file.close();
	return buffer;
}


vk::ShaderModule vkUtil::createModule(std::string filename, vk::Device device)
{
	std::vector<char> sourceCode = readFile(filename);
	vk::ShaderModuleCreateInfo moduleInfo = {};
	moduleInfo.flags = vk::ShaderModuleCreateFlags();
	moduleInfo.codeSize = sourceCode.size();
	moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

	try
	{
		return device.createShaderModule(moduleInfo);
	}
	catch (vk::SystemError err)
	{
		#ifndef Client_MODE
		std::string failedMsg = "Failed to create shader module for \"" + filename + "\"";
		TZE_ENGINE_ERR(failedMsg);
		#endif
	}
}