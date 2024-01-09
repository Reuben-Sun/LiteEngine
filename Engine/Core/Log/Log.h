#pragma once

#include <stdexcept>
#include <format>
#include <memory>
#include <spdlog/spdlog.h>

namespace Engine
{
    enum class LogLevel : uint8_t
    {
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    class Logger final
    {
    public:
        static Logger& getInstance()
        {
            static Logger instance;
            return instance;
        }

        template<typename... Args>
        void log(LogLevel level, Args&&... args)
        {
            switch (level)
            {
            case LogLevel::Debug:
                m_logger->debug(std::forward<Args>(args)...);
                break;
            case LogLevel::Info:
                m_logger->info(std::forward<Args>(args)...);
                break;
            case LogLevel::Warn:
                m_logger->warn(std::forward<Args>(args)...);
                break;
            case LogLevel::Error:
                m_logger->error(std::forward<Args>(args)...);
                break;
            case LogLevel::Fatal:
                m_logger->critical(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

    private:
        Logger();
        ~Logger();
        std::shared_ptr<spdlog::logger> m_logger;
    };
}