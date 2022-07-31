#pragma once
#include "pch.h"

namespace vkInit
{
	vk::Semaphore make_semaphore(vk::Device device);
	vk::Fence make_fence(vk::Device device);
}