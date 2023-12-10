#pragma once

//定义宏使输出文件名和行号
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_TRACE_ON
//#define SPDLOG_DEBUG_ON

#include <string>

#include "spdlog/spdlog.h"
//#include "spdlog/sinks/easy_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

#include "Singleton.h"

namespace PPEngine {
    namespace Core {
        class Logger : public Singleton<Logger> {
        public:
            Logger();
            ~Logger();

            bool Initialize() override;
            void Uninitialize() override;

            auto logger() const {
                return logPtr;
            }

        private:
            std::shared_ptr<spdlog::logger> logPtr;
        };

    }
}

//参见SPDLOG_LOGGER_CALL
#define SPDLOG_BASE(logger, level, ...) (logger)->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, level, __VA_ARGS__)
#define TRACELOG(...)     SPDLOG_BASE(PPEngine::Core::Logger::Get()->logger(), spdlog::level::trace, __VA_ARGS__)
#define DEBUGLOG(...)     SPDLOG_BASE(PPEngine::Core::Logger::Get()->logger(), spdlog::level::debug, __VA_ARGS__)
#define INFOLOG(...)      SPDLOG_BASE(PPEngine::Core::Logger::Get()->logger(), spdlog::level::info, __VA_ARGS__)
#define WARNLOG(...)      SPDLOG_BASE(PPEngine::Core::Logger::Get()->logger(), spdlog::level::warn, __VA_ARGS__)
#define ERRORLOG(...)     SPDLOG_BASE(PPEngine::Core::Logger::Get()->logger(), spdlog::level::err, __VA_ARGS__)
#define CRITICALLOG(...)  SPDLOG_BASE(PPEngine::Core::Logger::Get()->logger(), spdlog::level::critical, __VA_ARGS__)