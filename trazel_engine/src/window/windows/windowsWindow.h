#pragma once
#include "pch.h"


namespace tze
{
class windowsWindow
{
public:
	windowsWindow(int Width = 1280, int Height = 720, const char* Title = "TRAZEL_ENGINE");
	~windowsWindow();

	struct windowProps
	{

		GLFWwindow* window;
		uint32_t* width;
		uint32_t* height;
		const char* title;
	};

	windowProps* getWindowProps()
	{
		windowProps* thisWindowPros = new windowProps();
		thisWindowPros->window = window;
		thisWindowPros->width = &width;
		thisWindowPros->height = &height;
		thisWindowPros->title = title;
		return thisWindowPros;
	}

private:
	// the glfw window parameters:
	GLFWwindow* window;
	uint32_t width;
	uint32_t height;
	const char* title;
	bool framebufferResized;

	// creating glfw window
	void buildGLFWWindow(int width, int height);

	// change window props:
	bool wasWindowResized() { return framebufferResized; }
	////void resetWindowResizedFlag() { framebufferResized = false; }
	//static void framebufferResizedCallback(GLFWwindow* window, int width, int height);
	//void recreateSwapChain();
	//void recordCommendBuffer(int imageIndex);



	// ImGui:
	//ImGui_ImplVulkanH_Window mainWindowData;
	// uint32_t g_QueueFamily = (uint32_t)-1;
	//VkAllocationCallbacks* allocator;
	//void createImGui();
	//void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
	//tze::imGuiLayer imGui;
};
}