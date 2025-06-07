#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include <SDL3/SDL.h>

#include "fractal/FractalState.hpp"
#include "fractal/IFractal.hpp"

class UIManager
{
	public:
		UIManager(SDL_Window* window, SDL_GLContext context);
		~UIManager();

		void processEvent(const SDL_Event& event);
		void update(FractalState& state, const std::map<FractalType, std::unique_ptr<IFractal>>& fractals);
		void render();

		bool wantsCaptureMouse() const;
		bool wantsCaptureKeyboard() const;

		std::function<void()> onRequestRedraw;
		std::function<void(const std::string&, const std::string&, int)> onRequestScreenshot;
		std::function<void()> onQuit;

	private:
		char m_screenshotPathBuffer[256] = "fractavistas_shot.png";
		int m_screenshotFactorIndex = 1;
		int m_screenshotExtensionIndex = 0;
};
