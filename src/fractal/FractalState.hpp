#pragma once

#include <glm/vec2.hpp>

#include "FractalType.hpp"

struct FractalState
{
		int width = 1280;
		int height = 720;

		FractalType type = FractalType::Mandelbrot;
		glm::dvec2 offset = { -0.75, 0.0 };
		double zoom = 0.4;
		int maxIterations = 256;

		glm::dvec2 juliaConstant = { -0.8, 0.156 };

		bool needsUpdate = true;
};
