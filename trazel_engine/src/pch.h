#pragma once
#include "trazel_engine/log.h"
// window things:
#define GLFW_INCLUDE_VULKAN
#define VK_VERSION_1_0 
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <glfw3.h>
#include <glfw3native.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan.h>


#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>
#include <bitset>
#include <optional>
#include <set>
#include <fstream>


#ifdef TZE_PLATFORM_WINDOWS
	#define _WIN32_WINNT 0x0601
	#include <sdkddkver.h>
	// this defines remove some unused windows things:
	#ifndef FULL_WINTARD
	#define WIN32_LEAN_AND_MEAN
	#define NOGDICAPMASKS
	#define NOSYSMETRICS
	#define NOMENUS
	#define NOICONS
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	#define OEMRESOURCE
	#define NOATOM
	#define NOCLIPBOARD
	#define NOCOLOR
	#define NOCTLMGR
	#define NODRAWTEXT
	#define NOKERNEL
	#define NONLS
	#define NOMEMMGR
	#define NOMETAFILE
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOTEXTMETRIC
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define NORPC
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NOTAPE
	#endif

	#define NOMINMAX

	#define STRICT
	
	#include <windows.h>
	#include <wrl.h>
	#include <d3dcompiler.h>
	#include <d3d12.h>
	#include <d3d11.h>
#endif


