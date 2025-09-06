#pragma once

#include <compare>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

enum class FractalType
{
	Mandelbrot,
	Julia,
	BurningShip,
	CubicMandelbrot,
	Tricorn,
	Newton
};

struct FractalSpecificParams
{
		glm::dvec2 juliaConstant = { -0.8, 0.156 };
};

struct ColorStop
{
		glm::vec3 color{ 1.0f, 1.0f, 1.0f };
		float position = 0.0f;

		std::partial_ordering operator<=>(const ColorStop& other) const { return position <=> other.position; }

		bool operator==(const ColorStop& other) const = default;
};

struct ColoringParams
{
		bool useSmoothing = true;
		double paletteFrequency = 0.01f; // Factor for smoothing the color transitions.
		std::vector<ColorStop> palette = {
			{ { 0.55f, 0.75f, 0.95f }, 0.0f },

			{ { 0.65f, 0.8f, 1.0f }, 0.25f },

			{ { 0.7f, 0.9f, 0.95f }, 0.5f },

			{ { 0.8f, 0.95f, 1.0f }, 0.75f },

			{ { 0.92f, 0.97f, 1.0f }, 0.9f },

			{ { 1.0f, 1.0f, 1.0f }, 1.0f }
		};
};
