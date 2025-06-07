#include "Application.hpp"

#include <SDL3/SDL.h>

#include "utils/Logger.hpp"

Application::Application()
{
	Logger::Info("Initializing FractaVista");

	m_fractalState.width = m_width;
	m_fractalState.height = m_height;

	m_window = std::make_unique<Window>("FractaVista", m_width, m_height);

	m_fractalManager = std::make_unique<FractalManager>(m_width, m_height);
	m_inputHandler = std::make_unique<InputHandler>(m_fractalState);
	m_quadRenderer = std::make_unique<QuadRenderer>();
	m_uiManager = std::make_unique<UIManager>(m_window->getSDLWindow(), m_window->getGLContext());

	m_uiManager->onRequestRedraw = [this]() { m_fractalState.needsUpdate = true; };
	m_uiManager->onRequestScreenshot = [this](const std::string& path, const std::string& extension, int factor) {
		m_fractalManager->saveScreenshot(path, extension, m_fractalState, factor);
	};
	m_uiManager->onQuit = [this]() { m_isRunning = false; };
}

void Application::run()
{
	m_fractalState.needsUpdate = true; // Initial render
	while (m_isRunning)
	{
		processInput();
		update();
		render();
	}
}

void Application::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		m_uiManager->processEvent(event);
		if (!m_uiManager->wantsCaptureMouse() && !m_uiManager->wantsCaptureKeyboard())
		{
			m_inputHandler->processEvent(event);
		}

		if (event.type == SDL_EVENT_QUIT)
		{
			m_isRunning = false;
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED)
		{
			int w, h;
			SDL_GetWindowSize(m_window->getSDLWindow(), &w, &h);

			m_fractalState.width = w;
			m_fractalState.height = h;

			m_fractalManager->onResize(w, h);
			m_fractalState.needsUpdate = true;
		}
	}
}

void Application::update()
{
	m_uiManager->update(m_fractalState, m_fractalManager->getFractalMap());

	if (m_fractalState.needsUpdate)
	{
		m_fractalManager->generate(m_fractalState);
		m_fractalState.needsUpdate = false;
	}
}

void Application::render()
{
	m_window->prepareFrame();

	m_quadRenderer->render(m_fractalManager->getTexture());

	m_uiManager->render();

	m_window->swapBuffers();
}
