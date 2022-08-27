#pragma once
#include "pch.h"
#include "core.h"
#include "log.h"
#include "../window/windows/windowsWindow.h"
#include "layers/mainLayer/mainLyaer.h"
#include "layers/imguiLayer/imguiLayer.h"

namespace tze
{
	class TZE_API app
	{
	public:

		app();
		virtual ~app();

		void run();

		std::unique_ptr<windowsWindow> mainWindow;
		GLFWwindow* window;

		std::unique_ptr<mainLyaer> majorLay;
		std::unique_ptr<mainLyaer> majorLay1;
		std::unique_ptr<imguiLayer> imguiLay;
	};

	// this will be defiend in the client 
	app* createApp();


}

