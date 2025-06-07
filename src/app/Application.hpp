#pragma once

#include <memory>

#include "core/InputHandler.hpp"
#include "core/Window.hpp"
#include "fractal/FractalManager.hpp"
#include "fractal/FractalState.hpp"
#include "gfx/QuadRenderer.hpp"
#include "ui/UIManager.hpp"

class Application
{
	public:
		Application();
		void run();

	private:
		void processInput();
		void update();
		void render();

		const int m_width = 1280;
		const int m_height = 720;

		std::unique_ptr<Window> m_window;
		FractalState m_fractalState;
		std::unique_ptr<InputHandler> m_inputHandler;
		std::unique_ptr<FractalManager> m_fractalManager;
		std::unique_ptr<QuadRenderer> m_quadRenderer;
		std::unique_ptr<UIManager> m_uiManager;

		bool m_isRunning = true;
};
