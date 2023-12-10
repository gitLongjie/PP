#include "Logger.h"

#include <assert.h>

namespace PPEngine {
    namespace Core {
        Logger* Singleton<Logger>::instance_ = nullptr;

        Logger::Logger() {
            
        }

        Logger::~Logger() {
            
        }

        bool Logger::Initialize() {
            logPtr = spdlog::daily_logger_format_mt("daily_logger", "./ProjectorPlayer.log", 23, 59);

            logPtr->set_level(spdlog::level::trace);

            logPtr->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s %!:%#] %v");
            logPtr->flush_on(spdlog::level::trace);

            return true;
        }

        void Logger::Uninitialize() {

        }

    }
}
