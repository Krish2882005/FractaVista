#pragma once

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

class Logger
{
	public:
		template <typename... Args> static void Info(Args... args) { log("INFO", args...); }

		template <typename... Args> static void Warning(Args... args) { log("WARNING", args...); }

		template <typename... Args> static void Error(Args... args) { log("ERROR", args...); }

	private:
		template <typename... Args> static void log(const char* level, Args... args)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			std::cout << "[" << level << "] ";

			std::stringstream ss;
			(ss << ... << args);
			std::cout << ss.str() << std::endl;
		}

		static std::mutex m_mutex;
};

inline std::mutex Logger::m_mutex;
