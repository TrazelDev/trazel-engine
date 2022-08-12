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
	

	// functions for rezing 
	bool wasWindowResized() { return frameBufferResized; };
	void resetWindowResizedFlag() { frameBufferResized = false; };;
	static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
private:
	// the glfw window parameters:
	GLFWwindow* window;
	uint32_t width;
	uint32_t height;
	const char* title;
	bool frameBufferResized = false;
	// creating glfw window
	void buildGLFWWindow(int width, int height);

};
}