#pragma once
#include "pch.h"

namespace vkInit
{
	vk::Instance make_instance(const char* title);
	bool supported(std::vector<const char*> extentions, std::vector<const char*>& layers);
}