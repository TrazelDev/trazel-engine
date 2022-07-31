#include "pch.h"
#include "log.h"

namespace tze {
	std::shared_ptr<spdlog::logger> log::s_game_engineLogger;
	std::shared_ptr<spdlog::logger> log::s_clientLogger;

	void log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");	

		// the engine logger
		s_game_engineLogger = spdlog::stdout_color_mt("trazel_engine");
		s_game_engineLogger->set_level(spdlog::level::trace);
																													  
		// the client(app) logger
		s_clientLogger = spdlog::stdout_color_mt("app");
		s_clientLogger->set_level(spdlog::level::trace);
	}
	
}