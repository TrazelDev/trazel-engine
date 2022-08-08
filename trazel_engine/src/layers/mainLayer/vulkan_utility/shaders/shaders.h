#pragma once
#include "pch.h"

namespace vkUtil
{
	std::vector<char> readFile(std::string filename);
	vk::ShaderModule createModule(std::string filename, vk::Device device);
}
