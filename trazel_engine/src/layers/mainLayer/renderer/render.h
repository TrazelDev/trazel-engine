#pragma once
#include "../vulkan_setup/device/swapchain.h"

namespace tze
{
	class renderer
	{
	public:
		renderer(GLFWwindow* Window, const char* Title, uint32_t* Width, uint32_t* Height, const std::unique_ptr<windowsWindow>& MainWindow)
		{
			mainWindow = MainWindow.get();
		}
		~renderer();

		
	private:
		void makeCommands();
	};
}
