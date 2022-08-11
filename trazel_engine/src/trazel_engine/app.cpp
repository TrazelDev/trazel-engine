#include "pch.h"
#include "app.h"

namespace tze {
	app::app()
	{
		mainWindow = std::unique_ptr<windowsWindow>(new windowsWindow{ });
		windowsWindow::windowProps* windowProperties = mainWindow->getWindowProps();
		window = windowProperties->window;
		
		//imguiLay = new imguiLayer(windowProperties->window, windowProperties->title, *windowProperties->width, *windowProperties->height);
		//imguiLay->onAttach();

		majorLay = std::unique_ptr<mainLyaer>(new mainLyaer{ windowProperties->window, windowProperties->title, *windowProperties->width, *windowProperties->height, mainWindow });
		majorLay->onAttach();

		TZE_CLIENT_INFO("the window is up and running");
	}

	app::~app()
	{
		//delete imguiLay;
		majorLay.reset();
		mainWindow.reset();
		TZE_CLIENT_INFO("the app was successfully closed");
	}

	void app::run()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			majorLay->onUpdate();
			//imguiLay->onUpdate();
		}
		TZE_CLIENT_INFO("the window was closed successfully");
	}
}