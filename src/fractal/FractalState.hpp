#pragma once

#include <glm/vec2.hpp>

#include "FractalTypes.hpp"

struct FractalState
{
		int renderWidth = 1280;
		int renderHeight = 720;
		FractalType type = FractalType::Mandelbrot;
		glm::dvec2 offset = { -0.75, 0.0 };
		double zoom = 0.4;
		int maxIterations = 256;

		FractalSpecificParams specificParams;

		ColoringParams coloring;

		bool needsUpdate = true;
};
