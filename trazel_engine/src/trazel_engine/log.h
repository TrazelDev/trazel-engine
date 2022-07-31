#pragma once
#include "core.h"
#include "include\spdlog\sinks\stdout_color_sinks.h"
#include "include\spdlog\spdlog.h"

namespace tze {
	class TZE_API log
	{
	public:
		log() = default;
		~log() = default;

		static void init();


		inline static std::shared_ptr<spdlog::logger>& getGame_enginLogger() {
			return s_game_engineLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& getClientLogger() {
			return s_clientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_game_engineLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#ifndef Client_MODE

#define TZE_ENGINE_ERR(...)      ::tze::log::getGame_enginLogger()->error(__VA_ARGS__)
#define TZE_ENGINE_WARN(...)     ::tze::log::getGame_enginLogger()->warn(__VA_ARGS__)
#define TZE_ENGINE_INFO(...)     ::tze::log::getGame_enginLogger()->info(__VA_ARGS__)
#define TZE_ENGINE_TRC(...)      ::tze::log::getGame_enginLogger()->trace(__VA_ARGS__)

#define TZE_CLIENT_ERR(...)      ::tze::log::getClientLogger()->error(__VA_ARGS__)
#define TZE_CLIENT_WARN(...)     ::tze::log::getClientLogger()->warn(__VA_ARGS__)
#define TZE_CLIENT_INFO(...)     ::tze::log::getClientLogger()->info(__VA_ARGS__)
#define TZE_CLIENT_TRC(...)      ::tze::log::getClientLogger()->trace(__VA_ARGS__)

#else

#define TZE_ENGINE_ERR(...) 
#define TZE_ENGINE_WARN(...)
#define TZE_ENGINE_INFO(...)
#define TZE_ENGINE_TRC(...) 

#define TZE_CLIENT_ERR(...) 
#define TZE_CLIENT_WARN(...)
#define TZE_CLIENT_INFO(...)
#define TZE_CLIENT_TRC(...) 

#endif