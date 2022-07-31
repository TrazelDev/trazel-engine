#include "trazel_engine.h"

class App : public tze::app
{
public:
	App()
	{
	}
	~App()
	{
	}
};


tze::app* tze::createApp()
{
	return new App;
}