#pragma once

#include <cstdint>
#include <memory>

#include "core/Window.hpp"
#include "fractal/FractalComputer.hpp"
#include "fractal/FractalState.hpp"
#include "ui/UIManager.hpp"
#include "ui/UIState.hpp"

class Application
{
	public:
		Application();
		void run();

	private:
		void processInput();
		void update();
		void render();

		bool m_isRunning = true;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<FractalComputer> m_fractalComputer;
		std::unique_ptr<UIManager> m_uiManager;

		FractalState m_fractalState;
		UIState m_uiState;
};
