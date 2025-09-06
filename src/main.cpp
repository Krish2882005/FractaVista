#include <stdexcept>

#include "app/Application.hpp"
#include "util/Logger.hpp"

int main()
{
	Log::Init();
	Log::SetLevel(spdlog::level::info);

	int returnCode = 0;

	try
	{
		Application app;
		app.run();
	}
	catch (const std::exception& e)
	{
		FRACTAL_CRITICAL("A fatal error occurred and the application must close: {}", e.what());
		returnCode = 1;
	}
	catch (...)
	{
		FRACTAL_CRITICAL("An unknown fatal error occurred and the application must close.");
		returnCode = 1;
	}

	Log::Shutdown();

	return returnCode;
}
