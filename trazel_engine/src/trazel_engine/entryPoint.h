#pragma once

#ifdef TZE_PLATFORM_WINDOWS

	#ifdef DEBUG_MODE
		extern tze::app* tze::createApp();
		int main(int argc, char** argv)
		{
			tze::log::init();
			
			tze::app* App = tze::createApp();
			App->run();
			delete App;
		}
	#elif REALSE_MODE
		extern tze::app* tze::createApp();
		int main(int argc, char** argv)
		{
			tze::log::init();

			tze::app* App = tze::createApp();
			App->run();
			delete App;
		}
	#else	
		extern tze::app* tze::createApp();
		int WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR arg, int show_cmd)
		{
			tze::app* App = tze::createApp();
			App->run();
			delete App;

			return 0;
		}
		
	#endif
#endif

