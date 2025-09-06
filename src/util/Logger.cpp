#include "util/Logger.hpp"

#include <vector>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

struct Log::LogImpl
{
		std::shared_ptr<spdlog::logger> coreLogger;
};

std::unique_ptr<Log::LogImpl> Log::s_Impl;

void Log::Init()
{
	s_Impl = std::make_unique<LogImpl>();

	spdlog::init_thread_pool(8192, 1);

	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto rotating_file_sink
		= std::make_shared<spdlog::sinks::rotating_file_sink_mt>("fractal_viewer.log", 1024 * 1024 * 10, 5);
	auto error_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("fractal_viewer_errors.log", true);

	stdout_sink->set_level(spdlog::level::trace);
	rotating_file_sink->set_level(spdlog::level::trace);
	error_file_sink->set_level(spdlog::level::err);

	stdout_sink->set_pattern("[%^%l%$] %v");

	const std::string file_pattern = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] [%s:%#] %v";
	rotating_file_sink->set_pattern(file_pattern);
	error_file_sink->set_pattern(file_pattern);

	std::vector<spdlog::sink_ptr> sinks{ stdout_sink, rotating_file_sink, error_file_sink };

	s_Impl->coreLogger = std::make_shared<spdlog::async_logger>(
		"FRACTAL", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

	spdlog::register_logger(s_Impl->coreLogger);
	s_Impl->coreLogger->set_level(spdlog::level::trace);
	s_Impl->coreLogger->flush_on(spdlog::level::err);
}

void Log::Shutdown()
{
	s_Impl->coreLogger->flush();
	spdlog::shutdown();
	s_Impl.reset();
}

void Log::SetLevel(spdlog::level::level_enum level)
{
	s_Impl->coreLogger->set_level(level);
}

spdlog::logger& Log::GetCoreLogger()
{
	return *s_Impl->coreLogger;
}
