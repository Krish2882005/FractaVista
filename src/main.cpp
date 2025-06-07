#include <stdexcept>

#include "app/Application.hpp"
#include "utils/Logger.hpp"

int main(int argc, char* argv[])
{
	try
	{
		Application app;
		app.run();
	}
	catch (const std::exception& e)
	{
		Logger::Error("A fatal error occurred: ", e.what());
		return 1;
	}
	return 0;
}
