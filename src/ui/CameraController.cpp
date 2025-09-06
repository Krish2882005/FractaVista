#include <algorithm>
#include <cmath>

#include "CameraController.hpp"

CameraController::CameraController(double zoomSpeed, double panSpeed) : m_zoomSpeed(zoomSpeed), m_panSpeed(panSpeed)
{
}

void CameraController::setZoomLimits(const glm::dvec2& limits)
{

	m_zoomLimits.x = std::min(limits.x, limits.y);
	m_zoomLimits.y = std::max(limits.x, limits.y);
}

void CameraController::setPanLimits(const glm::dvec2& limits)
{

	m_panLimits.x = std::abs(limits.x);
	m_panLimits.y = std::abs(limits.y);
}

void CameraController::apply(FractalState& state, const Input& in)
{

	if (in.dragging)
	{

		state.offset.x -= (in.delta.x * m_panSpeed / in.viewportSize.y) / state.zoom;
		state.offset.y -= (in.delta.y * m_panSpeed / in.viewportSize.y) / state.zoom;
	}

	if (in.wheel != 0.0)
	{

		const double nx = (in.mousePos.x / in.viewportSize.x) - 0.5;
		const double ny = (in.mousePos.y / in.viewportSize.y) - 0.5;
		const double aspect = in.viewportSize.x / in.viewportSize.y;

		const double worldX_before = state.offset.x + (nx * aspect) / state.zoom;
		const double worldY_before = state.offset.y + (ny) / state.zoom;

		const double factor = (in.wheel > 0) ? m_zoomSpeed : (1.0 / m_zoomSpeed);
		const double oldZoom = state.zoom;
		state.zoom = std::clamp(oldZoom * factor, m_zoomLimits.x, m_zoomLimits.y);

		if (state.zoom != oldZoom)
		{
			state.offset.x = worldX_before - (nx * aspect) / state.zoom;
			state.offset.y = worldY_before - (ny) / state.zoom;
		}
	}

	state.offset.x = std::clamp(state.offset.x, -m_panLimits.x, m_panLimits.x);
	state.offset.y = std::clamp(state.offset.y, -m_panLimits.y, m_panLimits.y);
}
