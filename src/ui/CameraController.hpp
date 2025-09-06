#pragma once

#include <glm/vec2.hpp>

#include "fractal/FractalState.hpp"

class CameraController
{
	public:
		struct Input
		{
				bool dragging = false;
				glm::dvec2 delta;
				double wheel = 0.0;
				glm::dvec2 mousePos;
				glm::dvec2 viewportSize;
		};

		explicit CameraController(double zoomSpeed = 1.25, double panSpeed = 1.0);

		void apply(FractalState& state, const Input& in);

		void setZoomLimits(const glm::dvec2& limits);

		void setPanLimits(const glm::dvec2& limits);

	private:
		double m_zoomSpeed;
		double m_panSpeed;

		glm::dvec2 m_zoomLimits{ 1e-2, 1e+12 };
		glm::dvec2 m_panLimits{ 100.0, 100.0 };
};
