#pragma once
#include "core.h"
#include "log.h"
#include "../window/windows/windowsWindow.h"

namespace tze
{
	class TZE_API app
	{
	public:
		windowsWindow* mainWindow;

		app();
		virtual ~app();

		void run();
	};

	// this will be defiend in the client 
	app* createApp();
}

