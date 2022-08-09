#include "pch.h"
#include "app.h"

namespace tze {
	app::app()
	{
		mainWindow = new windowsWindow();
		windowsWindow::windowProps* windowProperties = mainWindow->getWindowProps();
		window = windowProperties->window;

		imguiLay = new imguiLayer(windowProperties->window, windowProperties->title, *windowProperties->width, *windowProperties->height);
		imguiLay->onAttach();

		//majorLay = new mainLyaer(windowProperties->window, windowProperties->title, *windowProperties->width, *windowProperties->height);
		//majorLay->onAttach();

		TZE_CLIENT_INFO("the window is up and running");
	}

	app::~app()
	{
		delete imguiLay;
		//delete majorLay;

		TZE_CLIENT_INFO("the engine was successfully closed");
	}

	void app::run()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			//majorLay->onUpdate();
			imguiLay->onUpdate();
		}
		TZE_CLIENT_INFO("the window was closed successfully");
	}
}