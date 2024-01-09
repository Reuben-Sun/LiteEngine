#pragma once

#include <iostream>
#include <format>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <sstream>
#include <optional>
#include <set>
#include <filesystem>
#include <fstream>
#include <chrono>

#include "Core/Log/Log.h"

#define BIT(x) (1 << x)

#define LOG_HELPER(LOG_LEVEL, ...) \
    Engine::Logger::getInstance().log(LOG_LEVEL,"[" __FUNCTION__ "] " +std::format(__VA_ARGS__));

#define LOG_DEBUG(...) LOG_HELPER(Engine::LogLevel::Debug, __VA_ARGS__);

#define LOG_INFO(...) LOG_HELPER(Engine::LogLevel::Info, __VA_ARGS__);

#define LOG_WARN(...) LOG_HELPER(Engine::LogLevel::Warn, __VA_ARGS__);

#define LOG_ERROR(...) LOG_HELPER(Engine::LogLevel::Error, __VA_ARGS__);

#define LOG_FATAL(...) LOG_HELPER(Engine::LogLevel::Fatal, __VA_ARGS__);