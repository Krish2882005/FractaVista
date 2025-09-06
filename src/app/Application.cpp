#include "Application.hpp"

#include <fstream>
#include <optional>

#include <SDL3/SDL.h>

#include "fractal/FractalDefinition.hpp"
#include "util/JsonUtils.hpp"
#include "util/Logger.hpp"
#include "util/PlatformUtils.hpp"

namespace
{
	constexpr int DEFAULT_WINDOW_WIDTH = 1280;
	constexpr int DEFAULT_WINDOW_HEIGHT = 720;
}

Application::Application()
{
	FRACTAL_INFO("Initializing FractaVista...");

	m_window = std::make_unique<Window>("FractaVista", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	m_fractalComputer = std::make_unique<FractalComputer>(DEFAULT_WINDOW_WIDTH,
														  DEFAULT_WINDOW_HEIGHT);
	m_uiManager = std::make_unique<UIManager>(m_window->getSDLWindow(), m_window->getGLContext());

	m_uiManager->onRequestRedraw = [this]() { m_fractalState.needsUpdate = true; };

	m_uiManager->onRequestScreenshot
		= [this](const ScreenshotRequest& request) { m_fractalComputer->saveScreenshot(request, m_fractalState); };

	m_uiManager->onQuit = [this]() { m_isRunning = false; };

	m_uiManager->onSavePreset = [this]() {
		const std::vector<nfdfilteritem_t> filter = { { .name = "FractaVista Preset", .spec = "fracta" } };

		auto path = PlatformUtils::SaveFileDialog(m_window.get(), "Save Preset", "preset.fracta", filter);

		if (path)
		{
			std::ofstream file(*path);
			if (file.is_open())
			{
				json j = m_fractalState;
				file << j.dump(4);
				FRACTAL_INFO("Preset saved to {}", *path);
			}
			else
			{
				FRACTAL_ERROR("Failed to open file for writing: {}", *path);
			}
		}
	};

	m_uiManager->onLoadPreset = [this]() {
		const std::vector<nfdfilteritem_t> filter = { { .name = "FractaVista Preset", .spec = "fracta" } };

		auto path = PlatformUtils::OpenFileDialog(m_window.get(), "Load Preset", "", filter);

		if (path)
		{
			std::ifstream file(*path);
			if (file.is_open())
			{
				try
				{
					json j;
					file >> j;
					m_fractalState = j.get<FractalState>();
					m_fractalState.needsUpdate = true;
					FRACTAL_INFO("Preset loaded from {}", *path);
				}
				catch (const json::exception& e)
				{
					FRACTAL_ERROR("Failed to parse preset file {}: {}", *path, e.what());
				}
			}
			else
			{
				FRACTAL_ERROR("Failed to open file for reading: {}", *path);
			}
		}
	};
}

void Application::run()
{
	m_fractalState.needsUpdate = true;
	while (m_isRunning)
	{
		processInput();
		update();
		render();
	}
	FRACTAL_INFO("FractaVista shutting down.");
}

void Application::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		m_uiManager->processEvent(event);
		if (event.type == SDL_EVENT_QUIT)
		{
			m_isRunning = false;
		}
	}
}

void Application::update()
{
	m_uiManager->update(m_fractalState, m_uiState, *m_fractalComputer);
}

void Application::render()
{
	if (m_fractalState.needsUpdate)
	{
		m_fractalComputer->generate(m_fractalState);
		m_fractalState.needsUpdate = false;
	}

	m_window->prepareFrame();
	m_uiManager->render();
	m_window->swapBuffers();
}
