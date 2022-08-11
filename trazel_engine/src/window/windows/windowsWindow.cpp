#include "pch.h"
#include "windowsWindow.h"

namespace tze
{
	windowsWindow::windowsWindow(int Width, int Height, const char* Title) : width(Width), height(Height), title(Title)
	{
		buildGLFWWindow(width, height);
	}

	windowsWindow::~windowsWindow()
	{
		glfwTerminate();
	}

	void windowsWindow::buildGLFWWindow(int width, int height)
	{
		// init glfw
		glfwInit();

		// no default rendering client vulkan will be hooked up later
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// resizing break swapchain will be disabled for now
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE /* GLFW_TRUE */);

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);   // you need to check if this procces succseeded later
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
	}

	void windowsWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto thisWindow = reinterpret_cast<windowsWindow*>(glfwGetWindowUserPointer(window));
		thisWindow->frameBufferResized = true;
		thisWindow->width = width;
		thisWindow->height = height;
	}
}