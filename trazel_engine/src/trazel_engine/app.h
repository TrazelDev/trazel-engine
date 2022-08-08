#pragma once
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
		windowsWindow* mainWindow;

		app();
		virtual ~app();

		void run();
	private:
		GLFWwindow* window;

		mainLyaer* majorLay;
		imguiLayer* imguiLay;
	};

	// this will be defiend in the client 
	app* createApp();


}

