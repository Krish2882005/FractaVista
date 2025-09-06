#pragma once

#include <memory>

#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

class Log
{
	public:
		Log() = delete;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(Log&&) = delete;

		static void Init();
		static void Shutdown();

		static void SetLevel(spdlog::level::level_enum level);
		static spdlog::logger& GetCoreLogger();

	private:
		struct LogImpl;
		static std::unique_ptr<LogImpl> s_Impl;
};

#define FRACTAL_TRACE(...)                                                                                             \
	::Log::GetCoreLogger().log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::trace,        \
							   __VA_ARGS__)

#define FRACTAL_INFO(...)                                                                                              \
	::Log::GetCoreLogger().log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::info,         \
							   __VA_ARGS__)

#define FRACTAL_WARN(...)                                                                                              \
	::Log::GetCoreLogger().log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::warn,         \
							   __VA_ARGS__)

#define FRACTAL_ERROR(...)                                                                                             \
	::Log::GetCoreLogger().log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::err,          \
							   __VA_ARGS__)

#define FRACTAL_CRITICAL(...)                                                                                          \
	::Log::GetCoreLogger().log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::critical,     \
							   __VA_ARGS__)
