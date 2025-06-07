#include "InputHandler.hpp"

#include <SDL3/SDL.h>

InputHandler::InputHandler(FractalState& state) : m_state(state)
{
}

void InputHandler::processEvent(const SDL_Event& event)
{
	switch (event.type)
	{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				m_isDragging = true;
				m_lastMouseX = static_cast<int>(event.button.x);
				m_lastMouseY = static_cast<int>(event.button.y);
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				m_isDragging = false;
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (m_isDragging)
			{
				double dx = static_cast<double>(event.motion.x - m_lastMouseX);
				double dy = static_cast<double>(event.motion.y - m_lastMouseY);
				double aspect = static_cast<double>(m_state.height) / m_state.width;

				m_state.offset.x -= (dx / m_state.height) / m_state.zoom;
				m_state.offset.y -= (dy / m_state.height) / m_state.zoom;

				m_lastMouseX = static_cast<int>(event.motion.x);
				m_lastMouseY = static_cast<int>(event.motion.y);
				m_state.needsUpdate = true;
			}
			break;

		case SDL_EVENT_MOUSE_WHEEL:
		{
			float mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			double aspect = static_cast<double>(m_state.width) / m_state.height;

			double uv_x = (static_cast<double>(mouseX) / m_state.width - 0.5) * aspect;
			double uv_y = static_cast<double>(mouseY) / m_state.height - 0.5;

			double mouseWorldX = m_state.offset.x + uv_x / m_state.zoom;
			double mouseWorldY = m_state.offset.y + uv_y / m_state.zoom;

			double zoomFactor = (event.wheel.y > 0) ? 1.2 : 1.0 / 1.2;
			m_state.zoom *= zoomFactor;

			m_state.offset.x = mouseWorldX - uv_x / m_state.zoom;
			m_state.offset.y = mouseWorldY - uv_y / m_state.zoom;

			m_state.needsUpdate = true;
			break;
		}
	}
}
