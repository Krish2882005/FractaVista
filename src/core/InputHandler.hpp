#pragma once

#include <SDL3/SDL_events.h>

#include "fractal/FractalState.hpp"

class InputHandler
{
	public:
		explicit InputHandler(FractalState& state);
		void processEvent(const SDL_Event& event);

	private:
		FractalState& m_state;

		bool m_isDragging = false;
		int m_lastMouseX = 0;
		int m_lastMouseY = 0;
};
