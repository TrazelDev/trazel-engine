#pragma once
#include "pch.h"
#include "window/windows/windowsWindow.h"

namespace tze
{
	class layer
	{
	public:
		layer() = default;
		virtual void onAttach() {};
		virtual void onDetach() {};
		virtual void onUpdate() {};
		virtual void onEvent() {};

		// windowProps:
		uint32_t width, height;
		const char* title;
		GLFWwindow* window;
	};
}


