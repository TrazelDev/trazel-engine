#include "pch.h"
#include "app.h"


namespace tze {
	app::app()
	{
		mainWindow = new windowsWindow();
		TZE_CLIENT_INFO("the window is up and running");
	}

	app::~app()
	{
	}

	void app::run()
	{
		
		mainWindow->run();
		/*
		if (codeMsg == -1)
			TZE_ENGINE_ERR("the window crasshed!");
		else
			TZE_CLIENT_INFO("closing the window succeeded");
		*/
	}
}